#include "server.h"

#include <sys/types.h>
#include <sys/socket.h>

#include "string_utils.h"

server::server(unsigned int port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
}

void server::start() {
    started = true;

    bind(sock, (sockaddr*) &address, sizeof(address));
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

    size_t new_size = recv(connection, buffer, buffer_size, 0);

    return std::string(buffer, new_size);
}

std::vector<std::string> server::receive_vector(size_t vector_size, size_t message_size) {
    std::string result = receive(message_size);

    trim(result);
    std::vector<std::string> parts = split_by_spaces(result);
    parts.resize(vector_size);

    return parts;
}