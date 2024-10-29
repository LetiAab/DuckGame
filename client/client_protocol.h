#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H
#include <sstream>
#include <string>

#include "common/socket.h"
#include "common/lobby_message.h"
#include "common/lobby_command.h"

#include <cstdint>


class ClientProtocol {
private:
    Socket skt;

public:
    explicit ClientProtocol(Socket&& skt);

    LobbyMessage recive_lobby_message();

    bool send_lobby_command(LobbyCommand lobbyCommand);

    void shutdown();

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ClientProtocol(ClientProtocol&&) = default;
    ClientProtocol& operator=(ClientProtocol&&) = default;
};

#endif
