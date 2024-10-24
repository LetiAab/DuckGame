#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <exception>
#include <iostream>
#include <list>
#include <sstream>

#include "common/socket.h"
#include "common/thread.h"
#include "lobby.h"


class Acceptor: public Thread {
private:
    Socket& skt_listener;
    bool is_alive;
    Lobby lobby;
    uint16_t ids_counter;

public:
    explicit Acceptor(Socket& skt);

    void run() override;

    void stop() override;

    Acceptor(const Acceptor&) = delete;
    Acceptor& operator=(const Acceptor&) = delete;

    Acceptor(Acceptor&&) = default;
    Acceptor& operator=(Acceptor&&) = default;
};

#endif
