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
#include "move_left_command.h"

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

    uint16_t player_id = 0;
    uint8_t type = 0;
    uint16_t match_id = 0;
    bool was_closed = false;

    skt.recvall(&player_id, sizeof(player_id), &was_closed);
    skt.recvall(&type, sizeof(type), &was_closed);
    skt.recvall(&match_id, sizeof(match_id), &was_closed);

    LobbyCommand cmd(player_id, type, match_id);

    return cmd;
}



std::shared_ptr<Executable> ServerProtocol::receive_command(){

    uint16_t player_id = 0;
    uint8_t type = 0;
    uint16_t match_id = 0;
    bool was_closed = false;

    skt.recvall(&player_id, sizeof(player_id), &was_closed);
    skt.recvall(&type, sizeof(type), &was_closed);
    skt.recvall(&match_id, sizeof(match_id), &was_closed);
    
    //aca deberia fijarme el type y devolver el comando que corresponda
    return std::make_shared<MoveLeftCommand>(player_id);

}


void ServerProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}

