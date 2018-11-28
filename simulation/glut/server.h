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
    std::vector<std::string> receive_vector(size_t vector_size, size_t message_size);

private:
    int sock;
    sockaddr_in address;
    int connection;

    bool started = false;
};

