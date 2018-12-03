#ifdef _WIN32
	#include <windows.h>
#endif
#include <GL/freeglut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <functional>
#include <algorithm>

#include "inverse_kinematics.h"
#include "nested_group.h"
#include "arms.h"
#include "mathematics.h"
#include "server.h"
#include "quaternion.h"
#include "serial.h"
#include "std_vector_utils.h"
#include "string_utils.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

float dt = 0.0f;

nested_group *arm;// Manipulator.
shape *cube; // Expected end effector position.

vector4 camera_rotation = {-90.0f, 1.0f, 0.0f, 0.0f};
vector3 camera_position = {0.0f, 0.0f, -100.0f};

unsigned int port = 7247;
server *net = new server(port); // Wi-Fi connection to bracer controller.
serial *usb = new serial("/dev/ttyUSB1"); // USB to manipulator controller.

void setup() {
    arm = make_planar_arm();

    cube = shape::make_cube();
    cube->color = {1.0f, 0.0f, 0.0f};

    net->start(); // Create server. Bracer controller will connect to this server.
    std::cout << "New connection." << std::endl;
    std::cout << "Waiting for gyroscope calibration." << std::endl;
    usleep(13 * 1000000);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(camera_position[0], camera_position[1], camera_position[2]);
    glRotatef(camera_rotation[0], camera_rotation[1], camera_rotation[2], camera_rotation[3]);

	arm->draw();
	cube->draw();

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

vector3 effector_position {0.0f, 0.0f, 0.0f};

void idle() {
    // Calculate delta time:
    static clock_t current_time = clock();
    static clock_t last_time = current_time;
    current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

	// Receive {q1, q2, q3, q4, a1, a2, a3} vector from bracer controller.
    std::string data_string = net->receive(1024);
    std::vector<std::string> data = split_by_spaces(data_string);
    std::vector<GLfloat> floats;
    std::transform(data.begin(), data.end(), floats.begin(), [](const std::string &s) -> GLfloat { return std::stof(s); });

    // Rotate acceleration and subtract gravity:
    quaternion q = quaternion(floats[0], floats[1], floats[2], floats[3]);
    auto acceleration = vector3 {floats[4], floats[5], floats[6]};
    acceleration = (q * acceleration) - vector3 {0.0f, 0.0f, 1.0f};

    // Adjust and correct end effector position:
    effector_position += acceleration * 5.0f;
    effector_position = effector_position.map([](const GLfloat &x) -> GLfloat { return clamp(x, -19.0f, 19.0f); });

    cube->translation = effector_position;
    cube->translation[2] = 0.0f;

    auto angles = inverse_kinematics_planar_arm(effector_position[0], effector_position[1], 10.0f, 10.0f, 10.0f);
    angles = angles.map(radians_to_degrees);

    arm->groups[0]->rotation = {angles[0], 0.0f, 0.0f, 1.0f};
    arm->groups[2]->rotation = {angles[1], 0.0f, 0.0f, 1.0f};
    arm->groups[4]->rotation = {angles[2], 0.0f, 0.0f, 1.0f};

    // Send angles to manipulator:
    std::string arm_message = std::to_string(angles[0]) + " "
                            + std::to_string(angles[1]) + " "
                            + std::to_string(angles[2]) + " \r";
    usb->write(arm_message.c_str());

    // Debug:
    std::cout << "DATA STRING" << data_string << std::endl;
    std::cout << "DATA VECTOR"; print_vector(data); std::cout << std::endl;
    std::cout << "ACCELERATION"; acceleration.print(); std::cout << std::endl;
    std::cout << "POSITION"; effector_position.print(); std::cout << std::endl;
    std::cout << "ANGLES"; angles.print(); std::cout << std::endl;
    std::cout << "ARM MESSAGE" << arm_message << std::endl;

	glutPostRedisplay();
}

void special(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            camera_rotation = {0.0f, 0.0f, 1.0f, 0.0f};
            camera_position = {0.0f, 0.0f, -100.0f};
            break;
        case GLUT_KEY_RIGHT:
            camera_rotation += {10.0f, 10.0f, 0.0f, 0.0f};
            break;
        case GLUT_KEY_LEFT:
            camera_rotation -= {10.0f, 10.0f, 0.0f, 0.0f};
            break;
        case GLUT_KEY_UP:
            camera_rotation += {0.0f, 10.0f, 0.0f, 0.0f};
            break;
        case GLUT_KEY_DOWN:
            camera_rotation -= {0.0f, 10.0f, 0.0f, 0.0f};
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
    glutSpecialFunc(special);
    glutReshapeFunc(reshape);

	glutMainLoop();

    return 0;
}
