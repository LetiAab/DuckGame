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
    bool was_closed = false;

    if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
        return false;
    }

    if (!skt.sendall(&message.type, sizeof(message.type), &was_closed) || was_closed) {
        return false;
    }

    if (!skt.sendall(&message.len_matches, sizeof(message.len_matches), &was_closed) || was_closed) {
        return false;
    }

    for (uint16_t match_id : message.existing_matches) {
        if (!skt.sendall(&match_id, sizeof(match_id), &was_closed) || was_closed) {
            return false;
        }
    }

    if (!skt.sendall(&message.current_match_id, sizeof(message.current_match_id), &was_closed) || was_closed) {
        return false;
    }

    return true;
}

LobbyCommand ServerProtocol::get_lobby_command(){


    LobbyCommand cmd;

    bool was_closed = false;

    skt.recvall(&cmd.player_id, 2, &was_closed);
    skt.recvall(&cmd.type, 1, &was_closed);
    
    //Este me esta bloqueando. Definir bien que es un command y un lobby command. 
    //Por que serian dos distintos?
    //skt.recvall(&cmd.match_id, 2, &was_closed);


    return cmd;
}


void ServerProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}
