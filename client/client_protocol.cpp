#include "client_protocol.h"


#include "../common/liberror.h"
#include "../common/constants.h"

ClientProtocol::ClientProtocol(Socket&& skt): skt(std::move(skt)) {}


Message ClientProtocol::recive_message(){
    
    bool was_closed = false;
    Message message;

    skt.recvall(&message.player_id, 2, &was_closed);
    skt.recvall(&message.type, 1, &was_closed);
    return message;
}

bool ClientProtocol::send_command(Command command){

    bool was_closed = false;

    if (!skt.sendall(&command.player_id, sizeof(command.player_id), &was_closed) || was_closed) {
        return false;
    }

    if (!skt.sendall(&command.type, sizeof(command.type), &was_closed) || was_closed) {
        return false;
    }

    return true;
}

void ClientProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}


