#ifndef SERVER_H
#define SERVER_H
#include <exception>
#include <iostream>
#include <sstream>

#include "../common/socket.h"
#include "server_protocol.h"

class Server {
private:
    const char* servname;

public:
    explicit Server(const char* servname);

    void run();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server(Server&&) = default;
    Server& operator=(Server&&) = default;
};

#endif
