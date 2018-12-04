#pragma once

#include <netinet/in.h>
#include <string>
#include <vector>

class server {
public:
    server(unsigned int port);

    void start();
    void stop();
    std::string receive(size_t message_size);

private:
    int sock;
    sockaddr_in sock_address;
    int connection;

    bool started = false;
};

