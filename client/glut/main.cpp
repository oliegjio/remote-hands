#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>

#include "shape.h"
#include "shape_group.h"
#include "nested_shape.h"
#include "matrix.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0;

auto hand = new nested_shape;
std::vector<shape_group*> groups;

void setup() {
    vec3 scale {1.0f, 3.0f, 1.0f};

	auto limb1 = shape::make_cube();
	limb1->scaling = scale;
	limb1->rotation = vec4 {-90.0f, 0.0f, 0.0f, 1.0f};
	limb1->color = vec3 {1.0f, 0.0f, 0.0f};
    auto rotor1 = shape::make_cube();
    rotor1->translation = {2.0f, 0.0f, 0.0f};
    rotor1->scaling = {0.5f, 2.0f, 0.5f};
    rotor1->color = vec3 {0.0f, 1.0f, 0.0f};

	auto limb2 = shape::make_cube();
	limb2->scaling = scale;
	limb2->color = vec3 {0.0f, 1.0f, 0.0f};
	auto rotor2 = shape::make_cube();
	rotor2->translation = {0.0f, 2.0f, 0.0f};
	rotor2->scaling = {0.5f, 0.5f, 2.0f};
	rotor2->color = vec3 {0.0f, 0.0f, 1.0f};

	auto limb3 = shape::make_cube();
	limb3->scaling = scale;
	limb3->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
	limb3->color = vec3 {0.0f, 0.0f, 1.0f};
    auto rotor3 = shape::make_cube();
    rotor3->translation = {-2.0f, 0.0f, 0.0f};
    rotor3->scaling = {0.5f, 0.5f, 2.0f};
    rotor3->color = vec3 {0.0f, 1.0f, 1.0f};

	auto limb4 = shape::make_cube();
	limb4->scaling = scale;
	limb4->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
	limb4->color = vec3 {0.0f, 1.0f, 1.0f};
    auto rotor4 = shape::make_cube();
    rotor4->translation = {-2.0f, 0.0f, 0.0f};
    rotor4->scaling = {0.5f, 0.5f, 2.0f};
    rotor4->color = vec3 {1.0f, 1.0f, 0.0f};

    auto limb5 = shape::make_cube();
    limb5->scaling = scale;
    limb5->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
    limb5->color = vec3 {1.0f, 1.0f, 0.0f};

	GLfloat hand_x = 5.0f;
    GLfloat hand_y = -5.0f;
    GLfloat hand_z = -40.0f;

    auto group1 = new shape_group {limb1, rotor1};
    group1->translation = {0.0f + hand_x, 0.0f + hand_y, 0.0f + hand_z};
    auto group2 = new shape_group {limb2, rotor2};
    group2->translation = {2.0f + hand_x, 6.0f + hand_y, 0.0f + hand_z};
    auto group3 = new shape_group {limb3, rotor3};
    group3->translation = {0.0f + hand_x, 12.0f + hand_y, 0.0f + hand_z};
    auto group4 = new shape_group {limb4, rotor4};
    group4->translation = {-8.0f + hand_x, 12.0f + hand_y, 0.0f + hand_z};
    auto group5 = new shape_group {limb5};
    group5->translation = {-16.0f + hand_x, 12.0f + hand_y, 0.0f + hand_z};

    groups = std::vector<shape_group*> {group1, group2, group3, group4, group5};
    hand = new nested_shape(groups);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	hand->draw();

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

float rotation = 0.0f;

void idle() {
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

	hand->rotation = {rotation, 0.0f, 1.0, 0.0f};
    hand->translation = {2.0f, 0.0, 0.0f};
    hand->group->shapes[1]->rotation = {rotation, 0.0f, 1.0f, 0.0f};

    hand->child->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->child->translation = {0.0f, 2.0f, 0.0f};
    hand->child->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    hand->child->child->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->child->child->translation = {-2.0f, 0.0f, 0.0f};
    hand->child->child->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    hand->child->child->child->rotation = {rotation, 0.0f, 0.0f, 1.0f};
    hand->child->child->child->translation = {-2.0f, 0.0f, 0.0f};
    hand->child->child->child->group->shapes[1]->rotation = {rotation, 0.0f, 0.0f, 1.0f};

    rotation += 1.0f;

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
