#pragma once

#include <netinet/in.h>
#include <string>

class server {
public:
    server(unsigned int port);

    void start();
    void stop();
    std::string receive();

private:
    int sock;
    sockaddr_in address;
    int connection;

    bool started = false;
};

