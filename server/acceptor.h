#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <exception>
#include <iostream>
#include <list>
#include <sstream>

#include "common/socket.h"
#include "common/thread.h"


class Acceptor: public Thread {
private:
    Socket& skt_listener;
    bool is_alive;

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
