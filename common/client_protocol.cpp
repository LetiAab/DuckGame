#include "client_protocol.h"


#include "../common/liberror.h"
#include "../common/constants.h"

ClientProtocol::ClientProtocol(Socket&& skt): skt(std::move(skt)) {}


LobbyMessage ClientProtocol::recive_lobby_message(){
    
    bool was_closed = false;
    LobbyMessage message;


    skt.recvall(&message.player_id, 2, &was_closed);
    skt.recvall(&message.type, 1, &was_closed);
    skt.recvall(&message.len_matches, 2, &was_closed);

    if(message.len_matches > 0){

        message.existing_matches.resize(message.len_matches);
        skt.recvall(message.existing_matches.data(), message.len_matches * sizeof(uint16_t), &was_closed);
    }

    skt.recvall(&message.current_match_id, 1, &was_closed);

    return message;
}

bool ClientProtocol::send_lobby_command(LobbyCommand command){

    bool was_closed = false;

    if (!skt.sendall(&command.player_id, sizeof(command.player_id), &was_closed) || was_closed) {
        return false;
    }

    if (!skt.sendall(&command.type, sizeof(command.type), &was_closed) || was_closed) {
        return false;
    }

    if (!skt.sendall(&command.match_id, sizeof(command.match_id), &was_closed) || was_closed) {
        return false;
    }

    return true;
}

void ClientProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}


