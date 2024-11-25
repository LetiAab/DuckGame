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
    case END_GAME:
        skt.recvall(&message.duck_winner, sizeof(message.duck_winner), &was_closed);
        break;

    case END_ROUND:

        skt.recvall(&message.duck_winner, sizeof(message.duck_winner), &was_closed);
        break;

    case FIRST_GAME_MESSAGE:
        //recibo el nuevo id del jugador
        skt.recvall(&message.player_id, 2, &was_closed);
        break;

    case MAP_INICIALIZATION:

        message.map.resize(MATRIX_N, std::vector<char>(MATRIX_M));
        for (size_t i = 0; i < MATRIX_N; ++i) { 
            skt.recvall(message.map[i].data(), MATRIX_M * sizeof(char), &was_closed); // Recibir cada fila
        }
        break;

    case BOX_DESTROYED:        
        skt.recvall(&message.box_id, sizeof(uint8_t), &was_closed);
        skt.recvall(&message.item_id, sizeof(uint8_t), &was_closed);
        break;

    case BOX_POSITION:

        skt.recvall(&message.box_id, sizeof(uint8_t), &was_closed);
        skt.recvall(&message.box_x, sizeof(int), &was_closed);
        skt.recvall(&message.box_y, sizeof(int), &was_closed);
        skt.recvall(&message.item_id, sizeof(uint8_t), &was_closed);
        break;
    

    case DUCKS_INICIALIZATION:
        skt.recvall(&message.ducks_quantity, sizeof(message.ducks_quantity), &was_closed);
        break;

    case ITEM_POSITION:
        skt.recvall(&message.item_id, sizeof(uint8_t), &was_closed);
        skt.recvall(&message.item_x, sizeof(int), &was_closed);
        skt.recvall(&message.item_y, sizeof(int), &was_closed);

        break;

    case SPAWN_PLACE_ITEM_UPDATE:
        skt.recvall(&message.spawn_place_id, sizeof(uint8_t), &was_closed);
        skt.recvall(&message.item_id, sizeof(uint8_t), &was_closed);
        break;
    
    case SPAWN_PLACE_POSITION:
        skt.recvall(&message.spawn_place_id, sizeof(uint8_t), &was_closed);
        skt.recvall(&message.spaw_place_x, sizeof(int), &was_closed);
        skt.recvall(&message.spaw_place_y, sizeof(int), &was_closed);
        skt.recvall(&message.item_id, sizeof(uint8_t), &was_closed);
        break;
    
    case DUCK_PICKUP_ITEM:
    case DUCK_EQUIP_ITEM:
        skt.recvall(&message.player_id, 2, &was_closed);
        skt.recvall(&message.item_id, sizeof(uint8_t), &was_closed);
        break;
        
    case KILL_DUCK:    
    case DROP_WEAPON:
    case SHOOT:
    case ARMOR_BROKEN:
    case HELMET_BROKEN:
        skt.recvall(&message.player_id, 2, &was_closed);
        break;

    case BULLET_POS_UPDATE:
        skt.recvall(&message.player_id, 2, &was_closed);
        skt.recvall(&message.bullet_x, sizeof(int), &was_closed);
        skt.recvall(&message.bullet_y, sizeof(int), &was_closed);
        skt.recvall(&message.bullet_id, sizeof(int), &was_closed);
        skt.recvall(&message.bullet_type, sizeof(int), &was_closed);

        break;

    case DUCK_POS_UPDATE:
        skt.recvall(&message.player_id, 2, &was_closed);
        skt.recvall(&message.duck_x, sizeof(int), &was_closed);
        skt.recvall(&message.duck_y, sizeof(int), &was_closed);
        skt.recvall(&message.looking, sizeof(char), &was_closed);
        skt.recvall(&message.is_moving, sizeof(bool), &was_closed);
        skt.recvall(&message.is_jumping, sizeof(bool), &was_closed);
        skt.recvall(&message.is_fluttering, sizeof(bool), &was_closed);
        skt.recvall(&message.is_laying_down, sizeof(bool), &was_closed);

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
    case SHOOT:
    case TAKE_ITEM:
    case DROP_WEAPON:

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


