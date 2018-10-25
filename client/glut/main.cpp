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

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0;

auto hand = new nested_shape;

void setup()
{
    vec3 scale {1.0f, 3.0f, 1.0f};

	auto limb1 = shape::make_cube();
	limb1->translation = vec3 {0.0f, -5.0f, -40.0f};
	limb1->scaling = scale;
	limb1->rotation = vec4 {-90.0f, 0.0f, 0.0f, 1.0f};
	limb1->color = vec3 {1.0f, 0.0f, 0.0f};

	auto limb2 = shape::make_cube();
	limb2->translation = vec3 {2.0f, 0.0f, -40.0f};
	limb2->scaling = scale;
	limb2->color = vec3 {0.0f, 1.0f, 0.0f};

	auto limb3 = shape::make_cube();
	limb3->translation = vec3 {0.0f, 5.0f, -40.0f};
	limb3->scaling = scale;
	limb3->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
	limb3->color = vec3 {0.0f, 0.0f, 1.0f};

	auto limb4 = shape::make_cube();
	limb4->translation = vec3 {-8.0f, 5.0f, -40.0f};
	limb4->scaling = scale;
	limb4->rotation = vec4 {90.0f, 0.0f, 0.0f, 1.0f};
	limb4->color = vec3 {0.0f, 1.0f, 1.0f};

    shape_group group1 {*limb1};
    shape_group group2 {*limb2};
    group2.rotation = {20.0f, 0.0f, 0.0f, 1.0f};
    shape_group group3 {*limb3};
    shape_group group4 {*limb4};

    std::vector<shape_group> groups {group1, group2, group3, group4};
    hand = new nested_shape(groups);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	hand->draw();

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void idle()
{
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

//    hand->rotation += {10.f * dt, 0.0f, 0.0f, 1.0f};

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
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
