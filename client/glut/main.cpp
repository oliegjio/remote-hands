#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>

#include "nested_shape.h"
#include "hands.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

#define PI 3.14159265f

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0;

nested_shape *hand;
shape *follower = shape::make_cube();

void setup() {
    hand = make_one_plane_hand();

    follower->scaling = {3.0f, 0.1f, 3.0f};
    follower->color = {1.0f, 0.0f, 0.0f};
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hand->draw();
//	follower->draw();
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

vec2 forward_kinematic(float angle1, float angle2, float angle3) {
	float l1 = 6.75f;
	float l2 = 6.75f;
	float l3 = 6.75f;
	return vec2 {
		l1 * std::cosf(angle1) + l2 * std::cosf(angle1 + angle2) + l3 * std::cosf(angle1 + angle2 + angle3),
		l1 * std::sinf(angle1) + l2 * std::sinf(angle1 + angle2) + l3 * std::sinf(angle1 + angle2 + angle3)
	};
}

void idle() {
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

	static float rotation = 0.0f;
	static bool rotation_turn = true;
	static float min_rotation = -45.0f;
	static float max_rotation = 0.0f;
	static float rotation_speed = 90.0f;

	hand->at(0)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
	hand->at(0)->translation = {0.0f, 2.0f, 0.0f};
	hand->at(0)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

	hand->at(1)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
	hand->at(1)->translation = {0.0f, 2.0f, 0.0f};
	hand->at(1)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

	hand->at(2)->rotation = {rotation, 0.0f, 0.0f, 1.0f};
	hand->at(2)->translation = {0.0f, 2.0f, 0.0f};
	hand->at(2)->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

	if (rotation < min_rotation) {
		rotation_turn = false;
	} else if (rotation > max_rotation) {
		rotation_turn = true;
	}

	if (rotation_turn) {
		rotation -= rotation_speed * dt;
	} else {
		rotation += rotation_speed * dt;
	}

	float rad_rotation = (rotation + 45.0f) * (PI / 180);
    vec2 coordinates = forward_kinematic(rad_rotation, rad_rotation, rad_rotation);
	coordinates.print();
	std::cout << std::endl;
	follower->rotation = {rotation, 0.0f, 0.0f, 1.0f};
	follower->translation = {coordinates[0], coordinates[1] - 8.0f, -65.0f};

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
	glutReshapeFunc(reshape);

	glutMainLoop();

    return 0;
}
