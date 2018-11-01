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
vec2 position = {30.0f, 0.0f};

vec4 camera_rotation = {0.0f, 0.0f, 1.0f, 0.0f};
vec3 camera_position = {0.0f, 0.0f, -100.0f};

void setup() {
    hand = make_4dof_hand();

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

vec2 forward_kinematic_planar_hand(float angle1, float angle2, float angle3) {
	float l1 = 10.0f;
	float l2 = 10.0f;
	float l3 = 10.0f;
	return vec2 {
		l1 * cosf(angle1) + l2 * cosf(angle1 + angle2) + l3 * cosf(angle1 + angle2 + angle3),
		l1 * sinf(angle1) + l2 * sinf(angle1 + angle2) + l3 * sinf(angle1 + angle2 + angle3)
	};
}

vec3 forward_kinematic_4dof_hand(float angle1, float angle2, float angle3, float angle4) {
    float l1 = 10.0f;
    float l2 = 10.0f;
    float l3 = 10.0f;
    float l4 = 10.0f;
    return vec3 {
        cosf(angle1) * (l2 * cosf(angle2) + l3 * cosf(angle2 + angle3)) + l4 * cosf(angle1) * cosf(angle2 + angle3 + angle4),
        sinf(angle1) * (l2 * cosf(angle2) + l3 * cosf(angle2 + angle3)) + l4 * sinf(angle1) * cosf(angle2 + angle3 + angle4),
        l1 + l2 * sinf(angle2) + l3 * sinf(angle2 + angle3) + l4 * sinf(angle2 + angle3 + angle4)
    };
}

vec4 inverse_kinematics_4dof_hand(float x, float y, float z, float angle1, float angle2, float angle3, float angle4, float phi) {
    float l1 = 10.0f;
    float l2 = 10.0f;
    float l3 = 10.0f;
    float l4 = 10.0f;
    float a = l3 * sinf(angle3);
    float b = l2 + l3 * cosf(angle3);
    float c = z - l1 - l4 * sinf(phi);
    float r = sqrtf(powf(a, 2.0f) + powf(b, 2.0f));
    float A = x - l4 * cosf(angle1) * cosf(phi);
    float B = y - l4 * sinf(angle1) * cosf(phi);
    float C = z - l1 - l4 * sinf(phi);

    float r1 = atanf(x / y);
    float r2 = atan2f(c, fabsf(sqrtf(powf(r, 2.0f) - powf(c, 2.0f)))) - atan2f(a, b);
    float r3 = acosf((powf(A, 2.0f) + powf(B, 2.0f) + powf(C, 2.0f) - powf(l2, 2.0f) - powf(l3, 2.0f)) / (2 * l2 * l3));
    float r4 = phi - r2 - r3;

    return vec4 {r1, r2, r3, r4};
}

void idle() {
	current_time = clock();
	dt = static_cast<float>(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;

	position = {20.0f, 20.0f};

//    static float rotation = 0.0f;
//    static bool rotation_turn = true;
//    static float min_rotation = -45.0f;
//    static float max_rotation = 0.0f;
//    static float rotation_speed = 90.0f;
    float angle1 = 0.0f;
    float angle2 = 0.0f + (PI / 4.0f);
    float angle3 = 0.0f;
    float angle4 = 0.0f;
    float phi = angle2 + angle3 + angle4;

    vec4 angles = inverse_kinematics_4dof_hand(position[0], position[1], 0.0f, angle1, angle2, angle3, angle4, phi);

//    angle1 = angles[0];
//    angle2 = angles[1];
//    angle3 = angles[2];
//    angle4 = angles[3];
//    phi = angles[1] + angles[2] + angles[3];

    hand->shapes[0]->rotation = {angles[0] * (180 / PI), 0.0f, 1.0f, 0.0f};
    hand->shapes[2]->rotation = {angles[1] * (180 / PI), 0.0f, 0.0f, 1.0f};
    hand->shapes[4]->rotation = {angles[2] * (180 / PI), 0.0f, 0.0f, 1.0f};
    hand->shapes[6]->rotation = {angles[3] * (180 / PI), 0.0f, 0.0f, 1.0f};

    follower->translation = {position[0], position[1], 0.0f};

	glutPostRedisplay();
}

float smooth_map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void passive_motion(int x, int y) {
    float range = 30.0f;
    float nx = smooth_map(x, 0, WIN_WIDTH, -range, range);
    float ny = -smooth_map(y, 0, WIN_HEIGHT, -range, range);
//    position = {nx, ny};
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
