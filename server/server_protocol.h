#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H
#include <sstream>
#include <string>

#include "common/socket.h"


class ServerProtocol {
private:
    Socket skt;

public:
    explicit ServerProtocol(Socket&& skt);

    //int send_data(const Message& message);

    //Command get_data();

    void shutdown();

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&&) = default;
    ServerProtocol& operator=(ServerProtocol&&) = default;
};

#endif
