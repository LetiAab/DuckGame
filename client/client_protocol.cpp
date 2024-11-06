#include "client_protocol.h"


#include "common/liberror.h"
#include "common/constants.h"
#include <iostream>

ClientProtocol::ClientProtocol(Socket&& skt): skt(std::move(skt)) {}


Message ClientProtocol::receive_message(){
    
    bool was_closed = false;
    Message message;

    skt.recvall(&message.type, 1, &was_closed);


    //ARMAR TODOS LOS CASOS
    switch (message.type)
    {
    case MAP_INICIALIZATION:
        message.map.resize(MATRIX_N, std::vector<char>(MATRIX_M));  
        for (size_t i = 0; i < MATRIX_N; ++i) { 
            skt.recvall(message.map[i].data(), MATRIX_M * sizeof(char), &was_closed); // Recibir cada fila
        }
    break;

    case DUCK_POS_UPDATE:
        skt.recvall(&message.player_id, 2, &was_closed);
        skt.recvall(&message.duck_x, sizeof(int), &was_closed);
        skt.recvall(&message.duck_y, sizeof(int), &was_closed);
        break;

    default:
        skt.recvall(&message.player_id, 2, &was_closed);
        skt.recvall(&message.len_matches, 2, &was_closed);

        if(message.len_matches > 0){

            message.existing_matches.resize(message.len_matches);
            skt.recvall(message.existing_matches.data(), message.len_matches * sizeof(uint16_t), &was_closed);
        }

        skt.recvall(&message.current_match_id, 1, &was_closed);
        break;
    }



    return message;
}

bool ClientProtocol::send_command(Command command){

    if(command.type == 0){
        return false;
    }

    bool was_closed = false;
    switch (command.type)
    {
    case MOVE_LEFT:
    case MOVE_RIGHT:
    case MOVE_DOWN:
    case MOVE_UP:
    case STOP_LEFT:
    case STOP_RIGHT:
    case STOP_UP:
    case STOP_DOWN:
        std::cout << "MANDO MOVIMIENTO" << "\n";

        if (!skt.sendall(&command.type, sizeof(command.type), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&command.player_id, sizeof(command.player_id), &was_closed) || was_closed) {
            return false;
        }
        
        break;
    
    default:
        if (!skt.sendall(&command.type, sizeof(command.type), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&command.player_id, sizeof(command.player_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&command.match_id, sizeof(command.match_id), &was_closed) || was_closed) {
            return false;
        }
        break;
    }



    return true;
}

void ClientProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}


