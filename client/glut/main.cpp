#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>

#include "nested_group.h"
#include "nested_shape.h"
#include "hands.h"
#include "kinematics.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

#define PI 3.14159265f

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0.0f;

nested_shape *hand;
shape *follower = shape::make_cube();
vec2 position = {0.0f, 0.0f};

vec4 camera_rotation = {0.0f, 0.0f, 1.0f, 0.0f};
vec3 camera_position = {0.0f, 0.0f, -100.0f};

void setup() {
    hand = make_planar_hand();

    follower->scaling = {0.5f, 0.5f, 3.0f};
    follower->color = {1.0f, 0.0f, 0.0f};
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(camera_position[0], camera_position[1], camera_position[2]);
    glRotatef(camera_rotation[0], camera_rotation[1], camera_rotation[2], camera_rotation[3]);

	hand->draw();
	follower->draw();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);

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

    animate_inverse_kinematics_planar_hand(hand, follower, position);
//    animate_forward_kinematics_planar_hand(hand, follower, dt);

	glutPostRedisplay();
}

float smooth_map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void passive_motion(int x, int y) {
    float range = 40.0f;
    float nx = smooth_map(x, 0, WIN_WIDTH, -range, range);
    float ny = -smooth_map(y, 0, WIN_HEIGHT, -range, range);
    position = {nx, ny};
}

void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            camera_rotation = {0.0f, 0.0f, 1.0f, 0.0f};
            camera_position = {0.0f, 0.0f, -100.0f};
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
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	const float light_position[4] = { 0.0f, 0.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

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
