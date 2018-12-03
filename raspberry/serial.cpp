#include "serial.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

serial::serial(const std::string &port, const speed_t &speed) {
    this->port = port;
    this->speed = speed;

    file_descriptor = open(this->port.c_str(), O_RDWR | O_NOCTTY| O_NDELAY);

    set_speed(default_speed);
}

void serial::write(const char *message) {
    ssize_t n_written = 0;
    ssize_t spot = 0;

    do {
        n_written = ::write(file_descriptor, &message[spot], 1);
        spot += n_written;
    } while (message[spot - 1] != '\r' && n_written > 0);
}

void serial::set_speed(speed_t speed) {
    this->speed = speed;

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    /* Error handling. */
    if (tcgetattr(file_descriptor, &tty) != 0) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    /* Set baud rate. */
    cfsetospeed(&tty, static_cast<speed_t>(this->speed));
    cfsetispeed(&tty, static_cast<speed_t>(this->speed));

    /* Setting other port stuff. */
    tty.c_cflag     &=  ~PARENB;            // Make 8n1.
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;           // No flow control.
    tty.c_cc[VMIN]   =  1;                  // Read doesn't block.
    tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout.
    tty.c_cflag     |=  CREAD | CLOCAL;     // Turn on READ & ignore ctrl lines.

    /* Make raw. */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes. */
    tcflush(file_descriptor, TCIFLUSH);
    if (tcsetattr(file_descriptor, TCSANOW, &tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr." << std::endl;
    }
}
