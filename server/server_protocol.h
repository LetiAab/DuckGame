#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H
#include <sstream>
#include <string>

#include "../common/socket.h"
#include "../common/lobby_message.h"
#include "../common/lobby_command.h"
#include "../common/command.h"
#include "executables/executable.h"
#include "executables/commands.h" //con esto incluyo todos los tipos de comandos


class ServerProtocol {
private:
    Socket skt;

public:
    explicit ServerProtocol(Socket&& skt);

    //int send_data(const Message& message);

    //Command get_data();

    bool send_lobby_message(const LobbyMessage& message);

    bool send_message(Message& message);

    LobbyCommand get_lobby_command();

    std::shared_ptr<Executable> receive_command();

    void shutdown();

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ServerProtocol(ServerProtocol&&) = default;
    ServerProtocol& operator=(ServerProtocol&&) = default;
};

#endif
