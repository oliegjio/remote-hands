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

#include "shape_groups/nested_group.h"
#include "tcp_cpp/tcp_server.h"
#include "serial_cpp/serial.h"
#include "std_utils.h"
#include "mathematics_c/mathematics.h"

#include "inverse_kinematics.h"
#include "arms.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

float dt = 0.0f; // Time between frames.

nested_group *arm;// Manipulator.
shape *end_effector; // Expected end effector position.

tcp_server *net; // Wi-Fi connection to bracer controller.
serial *usb; // USB to manipulator controller.

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    vector4 camera_rotation = {0.0f, 1.0f, 0.0f, 0.0f};
    vector3 camera_position = {0.0f, 0.0f, -100.0f};

    // Apply camera transforms:
    glTranslatef(camera_position[0], camera_position[1], camera_position[2]);
    glRotatef(camera_rotation[0], camera_rotation[1], camera_rotation[2], camera_rotation[3]);

    // Draw scene objects:
    arm->draw();
    end_effector->draw();

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    // Make perspective projection:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 500.0f);
    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

float getDeltaTime() {
    static clock_t current_time = clock();
    static clock_t last_time = current_time;
    current_time = clock();
    dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
    last_time = current_time;

    return dt;
}

void idle() {
    dt = getDeltaTime();

//	// Receive {q1, q2, q3, q4, a1, a2, a3} vector from bracer controller.
//    std::string data_string = net->receive(1024);
//    std::vector<std::string> data = split_by_spaces(data_string);
//    data.resize(7);
//    std::vector<GLfloat> floats(7);
//    try {
//        std::transform(data.begin(), data.end(), floats.begin(), [](const std::string &s) { return std::stof(s); });
//    } catch (const std::exception &e) {
//        return;
//    }
//
//    // Rotate acceleration and subtract gravity:
//    quaternion q = quaternion(floats[0], floats[1], floats[2], floats[3]);
//    auto acceleration = vector3 {floats[4], floats[5], floats[6]};
//    acceleration = q * acceleration;
//    acceleration -= vector3 {0.0f, 0.0f, 1.0f};

    static vector3 effector_position {0.0f, 0.0f, 0.0f}; // Manipulator end effector position;
    effector_position[0] = sinf(clock()) * 10000 * dt;
    effector_position[1] = cosf(clock()) * 10000 * dt;

//    // Adjust and correct end effector position:
//    effector_position += acceleration * 3.0f;
//    effector_position = effector_position.map([](const GLfloat &x) { return clamp(x, -19.0f, 19.0f); });

    // Move cube to expected end effector position:
    end_effector->translation[0] = effector_position[0];
    end_effector->translation[1] = effector_position[1];
    end_effector->translation[2] = 0.0f;

    // Calculate inverse kinematics:
    auto angles = inverse_kinematics_planar_arm(effector_position[0], effector_position[1], 10.0f, 10.0f, 10.0f);
    angles = angles.map(radians_to_degrees);

    // Set calculated angles to manipulator manipulator_simulation:
    arm->groups[0]->rotation = {angles[0], 0.0f, 0.0f, 1.0f};
    arm->groups[2]->rotation = {angles[1], 0.0f, 0.0f, 1.0f};
    arm->groups[4]->rotation = {angles[2], 0.0f, 0.0f, 1.0f};

    // Send angles to manipulator:
    std::string arm_message = std::to_string(angles[0]) + " "
                              + std::to_string(angles[1]) + " "
                              + std::to_string(angles[2]) + " \r";
//    usb->write(arm_message.c_str());

//    // Debug:
//    std::cout << "DATA STRING " << data_string << std::endl;
//    std::cout << "DATA VECTOR "; print_vector(data); std::cout << std::endl;
//    std::cout << "ACCELERATION "; acceleration.print(); std::cout << std::endl;
//    std::cout << "POSITION "; effector_position.print(); std::cout << std::endl;
//    std::cout << "ANGLES "; angles.print(); std::cout << std::endl;
//    std::cout << "ARM MESSAGE " << arm_message << std::endl;
//    std::cout << std::endl;

    glutPostRedisplay();
}

int main(int argc, char **argv) {
    /**
     * Setup:
     */

    unsigned int tcp_server_port = 7247;
    const char *tty = "/dev/ttyUSB0";

//    net = new tcp_server(tcp_server_port);
//    usb = new serial(tty);

    arm = make_planar_arm();

    end_effector = shape::make_cube();
    end_effector->color = {1.0f, 0.0f, 0.0f};

//    net->start(); // Create tcp_server. Bracer controller will connect to this tcp_server.
//    std::cout << "New connection." << std::endl;

    /**
     * OpenGL + GLUT setup:
     */

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
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}