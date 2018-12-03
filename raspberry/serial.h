#pragma once

#include <string>
#include <termios.h>

class serial {
public:
    serial(const std::string &tty_path, const speed_t &speed = B9600);
    void write(const char *message);
    void set_speed(speed_t speed);

private:
    std::string tty_path;
    int file_descriptor;
    speed_t speed;
    speed_t default_speed = B9600;
};
