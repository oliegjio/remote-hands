#include "server.h"

#include <sys/types.h>
#include <sys/socket.h>

server::server(unsigned int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    sock_address.sin_family = AF_INET;
    sock_address.sin_port = htons(port);
    sock_address.sin_addr.s_addr = INADDR_ANY;
}

void server::start() {
    started = true;

    bind(sock, (sockaddr*) &sock_address, sizeof sock_address);
    listen(sock, 1);

    connection = accept(sock, NULL, NULL);
}

void server::stop() {
    started = false;
    shutdown(sock, 2);
}

std::string server::receive(size_t message_size) {
    if (!started) {
        return std::string("");
    }

    size_t buffer_size = message_size;
    char buffer[buffer_size];

    ssize_t new_size = recv(connection, buffer, buffer_size, 0);

    return std::string(buffer, new_size);
}