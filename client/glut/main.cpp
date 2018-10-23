#ifdef _WIN32
    #include <windows.h>
#endif
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>

#include "shape.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

clock_t current_time = clock();
clock_t last_time = current_time;
float dt = 0;

std::vector<shape*> shapes;

void setup()
{
	auto shape1 = shape::make_cube();
	shape1->translate(-2, 0, 0);
	shapes.push_back(shape1);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, -2.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
	glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)

	glVertex3f(1.0f, -1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Bottom)
	glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Bottom)

	glVertex3f(1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
	glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Front)

	glVertex3f(1.0f, -1.0f, -1.0f);    // Top Right Of The Quad (Back)
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f, -1.0f);    // Bottom Left Of The Quad (Back)
	glVertex3f(1.0f, 1.0f, -1.0f);    // Bottom Right Of The Quad (Back)

	glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f, -1.0f);    // Top Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, -1.0f);    // Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Left)

	glVertex3f(1.0f, 1.0f, -1.0f);    // Top Right Of The Quad (Right)
	glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, -1.0f);    // Bottom Right Of The Quad (Right)
	glEnd();
	
	/*glColor3f(1.0, 0.0, 0.0);
	for (const auto &shape : shapes)
	{
		shape->draw();
	}*/

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	const int side = std::fmin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, width / height, 0.1f, 100.0f);
	//glFrustum(-2, 2, -2, 2, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void idle()
{
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;
	std::cout << dt << "\n";
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	setup();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Remote Hands");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	const float position[4] = { 0.0f, 0.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	glutMainLoop();

    return 0;
}
