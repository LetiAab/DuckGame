#include "acceptor.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "common/socket.h"


Acceptor::Acceptor(Socket& skt):
        skt_listener(skt), is_alive(true){}

void Acceptor::run() {

   }

void Acceptor::stop() {
    is_alive = false;

}
