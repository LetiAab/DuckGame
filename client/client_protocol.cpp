#include "client_protocol.h"


#include "common/liberror.h"
#include "common/constants.h"
#include <iostream>

ClientProtocol::ClientProtocol(Socket&& skt): skt(std::move(skt)) {}


Message ClientProtocol::recive_message(){
    
    bool was_closed = false;
    Message message;

    skt.recvall(&message.player_id, 2, &was_closed); //hay que cambiar para recibir primero el type
    skt.recvall(&message.type, 1, &was_closed);


    //ARMAR TODOS LOS CASOS
    switch (message.type)
    {
    case MAP_INICIALIZATION:
        message.map.resize(10, std::vector<int>(15));  // Cambia las dimensiones aquí
        for (size_t i = 0; i < 10; ++i) { // 10 filas
            skt.recvall(message.map[i].data(), 15 * sizeof(int), &was_closed); // Recibir cada fila
        }
    break;
    
    default:

        skt.recvall(&message.len_matches, 2, &was_closed);

        if(message.len_matches > 0){

            message.existing_matches.resize(message.len_matches);
            skt.recvall(message.existing_matches.data(), message.len_matches * sizeof(uint16_t), &was_closed);
        }

        skt.recvall(&message.current_match_id, 1, &was_closed);
    }



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

    if (!skt.sendall(&command.match_id, sizeof(command.match_id), &was_closed) || was_closed) {
        return false;
    }

    return true;
}

void ClientProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}


