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

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

#define PI 3.14159265f

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0.0f;

nested_shape *hand;
shape *follower = shape::make_cube();

void setup() {
    hand = make_one_plane_hand();
    hand->rotation = {-90.0f, 0.0f, 0.0f, 1.0f};

    follower->scaling = {0.5f, 0.5f, 3.0f};
    follower->color = {1.0f, 0.0f, 0.0f};
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(0.0f, 0.0f, -75.0f);

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

vec2 forward_kinematic(float angle1, float angle2, float angle3) {
	float l1 = 10.0f;
	float l2 = 10.0f;
	float l3 = 10.0f;
	return vec2 {
		l1 * std::cosf(angle1) + l2 * std::cosf(angle1 + angle2) + l3 * std::cosf(angle1 + angle2 + angle3),
		l1 * std::sinf(angle1) + l2 * std::sinf(angle1 + angle2) + l3 * std::sinf(angle1 + angle2 + angle3)
	};
}

void idle() {
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

    static float rotation_1 = 0.0f;
    static float rotation_2 = 0.0f;
    static float rotation_3 = 0.0f;
    static float phi = rotation_1 + rotation_2 + rotation_3;
    static vec2 position = {30.0f, 0.0f};

    float l1, l2, l3;
    l1 = l2 = l3 = 10.0f;

    float cos = (std::powf(position[0], 2.0f) + std::powf(position[1], 2.0f) - std::powf(l1, 2.0f) - std::powf(l2, 2.0f)) / (2 * l1 * l2);
    float sin = std::sqrtf(1 - std::powf(std::cosf(rotation_2), 2.0f));
    float k1 = l1 + l2 * std::cosf(rotation_2);
    float k2 = l2;
    float xn = position[0] - l3 * std::cosf(phi);
    float yn = position[1] - l3 * std::sinf(phi);

    float new_rotation_1 = std::atan2f(k1 * yn - k2 * xn, k1 * xn - k2 * yn);
    float new_rotation_2 = std::atan2f(sin, cos);
    float new_rotation_3 = phi - (rotation_1 + rotation_2);

    rotation_1 = new_rotation_1;
    rotation_2 = new_rotation_2;
    rotation_3 = new_rotation_3;
    phi = rotation_1 + rotation_2 + rotation_3;

    hand->shapes[0]->rotation = {rotation_1, 0.0f, 0.0f, 1.0f};
    hand->shapes[2]->rotation = {rotation_2, 0.0f, 0.0f, 1.0f};
    hand->shapes[4]->rotation = {rotation_3, 0.0f, 0.0f, 1.0f};

    follower->translation = {position[0], position[1], 0};

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
