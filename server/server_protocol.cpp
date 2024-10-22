#include "server_protocol.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "common/liberror.h"

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}


void ServerProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}
