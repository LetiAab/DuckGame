#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H
#include <sstream>
#include <string>

#include "../common/socket.h"
#include "../common/message.h"
#include "../common/command.h"

#include <cstdint>


class ClientProtocol {
private:
    Socket skt;

public:
    explicit ClientProtocol(Socket&& skt);

    Message receive_message();

    bool send_command(Command lobbyCommand);

    void shutdown();

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&&) = default;
    ClientProtocol& operator=(ClientProtocol&&) = default;
};

#endif
