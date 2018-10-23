#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>

#include "shape.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0;

std::vector<shape*> shapes;

void setup()
{
	auto shape1 = shape::make_cube();
	shape1->translation = vec3{ 0.0f, 0.0f, -10.0f };
	shape1->scaling = vec3{ 2.0f, 2.0f, 2.0f };
	shape1->color = vec3{ 0.0f, 1.0f, 0.0f };
	shape1->rotation = vec4{ 30.0f, 1.0f, 1.0f, 0.0f };
	shapes.push_back(shape1);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (const auto & shape : shapes)
	{
		shape->draw();
	}

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
