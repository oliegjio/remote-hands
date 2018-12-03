#pragma once

#include <string>
#include <termios.h>

struct serial {
    std::string port;
    int speed;
    int file_descriptor;
    speed_t default_speed = B9600;

    serial(const std::string &port, const speed_t &speed = B9600);

    void write(const char *message);

    void set_speed(speed_t speed);
};
