#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>

#include "kinematics_animations.h"
#include "nested_group.h"
#include "arms.h"
#include "mathematics.h"
#include "server.h"
#include "utils.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0.0f;

nested_group *arm;
shape *follower = shape::make_cube();
vector3 position = {0.0f, 0.0f, 0.0f};

vector4 camera_rotation = {0.0f, 0.0f, 0.0f, 1.0f};
vector3 camera_position = {0.0f, 0.0f, -100.0f};

size_t window_width = WIN_WIDTH;
size_t window_height = WIN_HEIGHT;

server *net = new server(5677);

void setup() {
    arm = make_planar_arm();

    follower->scaling = {0.5f, 0.5f, 3.0f};
    follower->color = {1.0f, 0.0f, 0.0f};

    net->start();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(camera_position[0], camera_position[1], camera_position[2]);
    glRotatef(camera_rotation[0], camera_rotation[1], camera_rotation[2], camera_rotation[3]);

	arm->draw();
	follower->draw();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void idle() {
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

//    animate_inverse_kinematics_planar_arm(arm, follower, position);
//    follower->translation = position;

    std::string message = net->receive(); // Receive angles from Python backend.
    trim(message);
    std::vector<std::string> parts = split_by_spaces(message);

    std::vector<GLfloat> angles;
    for (size_t i = 0; i < parts.size(); i++) {
        angles.push_back(std::stof(parts[i]));
    }

    arm->groups[0]->rotation = {angles[0], 0.0f, 0.0f, 1.0f};
    arm->groups[2]->rotation = {angles[1], 0.0f, 0.0f, 1.0f};
    arm->groups[4]->rotation = {angles[2], 0.0f, 0.0f, 1.0f};

	glutPostRedisplay();
}

void passive_motion(int x, int y) {
    float range = 30.0f;
    float nx = smooth_map(x, 0, window_width, -range, range);
    float ny = -smooth_map(y, 0, window_height, -range, range);
    position = {nx, ny, 0.0f};
}

void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            camera_rotation = {0.0f, 0.0f, 1.0f, 0.0f};
            camera_position = {0.0f, 0.0f, -100.0f};
            break;
        case GLUT_KEY_F2:
            position += {0.0f, 3.0f, 0.0f};
            break;
        case GLUT_KEY_F3:
            position -= {0.0f, 3.0f, 0.0f};
            break;
        case GLUT_KEY_RIGHT:
            camera_rotation += {22.5f, 0.0f, 0.0f, 0.0f};
            break;
        case GLUT_KEY_LEFT:
            camera_rotation -= {22.5f, 0.0f, 0.0f, 0.0f};
            break;
        case GLUT_KEY_UP:
            camera_position += {0.0f, 0.0f, 5.0f};
            break;
        case GLUT_KEY_DOWN:
            camera_position -= {0.0f, 0.0f, 5.0f};
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char **argv) {
	setup();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Remote Hands");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
    glutPassiveMotionFunc(passive_motion);
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);

	glutMainLoop();

    return 0;
}
