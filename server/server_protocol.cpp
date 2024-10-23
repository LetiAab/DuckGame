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

bool ServerProtocol::send_lobby_message(const LobbyMessage& message){
    //mandar el mensaje por el socket bien
    bool was_closed = false;
    skt.sendall(&message, 10, &was_closed);

    return true;
}

LobbyCommand ServerProtocol::get_lobby_command(){
    //recibir el mensaje por el socket
    LobbyCommand cmd;
    cmd.type = 0; 

    return cmd;
}


void ServerProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}
