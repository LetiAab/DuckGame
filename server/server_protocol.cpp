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


#include "common/constants.h"

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}

bool ServerProtocol::send_lobby_message(const LobbyMessage& message){
    bool was_closed = false;
    
    if (!skt.sendall(&message.type, sizeof(message.type), &was_closed) || was_closed) {
        return false;
    }
    
    if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
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

bool ServerProtocol::send_message(Message& message){
    bool was_closed = false;

    //mando el type primero siempre
    if (!skt.sendall(&message.type, sizeof(message.type), &was_closed) || was_closed) {
            return false;
        }

    switch (message.type)
    {
    case END_GAME:
        if (!skt.sendall(&message.duck_winner, sizeof(message.duck_winner), &was_closed) || was_closed) {
            return false;
        }
        break;

    case END_ROUND:

        if (!skt.sendall(&message.duck_winner, sizeof(message.duck_winner), &was_closed) || was_closed) {
            return false;
        }
        break;

    case FIRST_GAME_MESSAGE:
        //le envio el NUEVO ID, que usara en la partida
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    case MAP_INICIALIZATION:
        // Enviar la matriz fila por fila
        for (size_t i = 0; i < MATRIX_N; ++i) {
            if (!skt.sendall(message.map[i].data(), MATRIX_M * sizeof(char), &was_closed) || was_closed) {
                return false;
            }
        }

        break;

    case DUCKS_INICIALIZATION:
        // Enviar la cantidad de patos
        if (!skt.sendall(&message.ducks_quantity, sizeof(message.ducks_quantity), &was_closed) || was_closed) {
            return false;
        }

        break;

    case DUCK_PICKUP_ITEM:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    case DUCK_EQUIP_ITEM:

        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    case BOX_POSITION:

        std::cout << "MANDO LA CAJA" << "\n";


        if (!skt.sendall(&message.box_id, sizeof(message.box_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.box_x, sizeof(message.box_x), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.box_y, sizeof(message.box_y), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }

        break;

    case BOX_DESTROYED:
        if (!skt.sendall(&message.box_id, sizeof(message.box_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    
    case DROP_WEAPON:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        break;
    
    case ARMOR_BROKEN:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    case HELMET_BROKEN:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    case SHOOT:
        std::cout << "sale el mensaje de shoot \n";
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        break;
    case ITEM_POSITION:
        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_x, sizeof(message.item_x), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_y, sizeof(message.item_y), &was_closed) || was_closed) {
            return false;
        }

        break;

    case SPAWN_PLACE_ITEM_UPDATE:
        if (!skt.sendall(&message.spawn_place_id, sizeof(message.spawn_place_id), &was_closed) || was_closed) {
            return false;
        }
        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }
        break;

    
    case SPAWN_PLACE_POSITION:
 
        if (!skt.sendall(&message.spawn_place_id, sizeof(message.spawn_place_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.spaw_place_x, sizeof(message.spaw_place_x), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.spaw_place_y, sizeof(message.spaw_place_y), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            return false;
        }

        break;


    case BULLET_POS_UPDATE:

        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.bullet_x, sizeof(message.bullet_x), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.bullet_y, sizeof(message.bullet_y), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.bullet_id, sizeof(message.bullet_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.bullet_type, sizeof(message.bullet_type), &was_closed) || was_closed) {
            return false;
        }
        break; 

    case KILL_DUCK:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        break;


    case DUCK_POS_UPDATE:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.duck_x, sizeof(message.duck_x), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.duck_y, sizeof(message.duck_y), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.looking, sizeof(message.looking), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.is_moving, sizeof(message.is_moving), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.is_jumping, sizeof(message.is_jumping), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.is_fluttering, sizeof(message.is_fluttering), &was_closed) || was_closed) {
            return false;
        }
        if (!skt.sendall(&message.is_laying_down, sizeof(message.is_laying_down), &was_closed) || was_closed) {
            return false;
        }

    default:
        break;
    }

    return true;
}


LobbyCommand ServerProtocol::get_lobby_command(){

    uint16_t player_id = 0;
    uint8_t type = 0;
    uint16_t match_id = 0;
    bool was_closed = false;

    skt.recvall(&type, sizeof(type), &was_closed);
    skt.recvall(&player_id, sizeof(player_id), &was_closed);
    skt.recvall(&match_id, sizeof(match_id), &was_closed);

    LobbyCommand cmd(player_id, type, match_id);

    return cmd;
}



std::shared_ptr<Executable> ServerProtocol::receive_command(){

    uint16_t player_id = 0;
    uint8_t type = 0;
    bool was_closed = false;

    skt.recvall(&type, sizeof(type), &was_closed);
    skt.recvall(&player_id, sizeof(player_id), &was_closed);
    
    //aca deberia fijarme el type y devolver el comando que corresponda

    //std::cout << "Comando recibido: " << std::endl;
    //std::cout << "  Player ID: " << player_id << std::endl;
    //std::cout << "  Type: " << static_cast<int>(type) << std::endl; // Convertir a int para mostrar el valor de uint8_t

    switch (type)
    {
    case MOVE_LEFT:
        return std::make_shared<MoveLeftCommand>(player_id);
    case MOVE_RIGHT:
        return std::make_shared<MoveRightCommand>(player_id);
    case MOVE_UP:
        return std::make_shared<MoveUpCommand>(player_id);
    case MOVE_DOWN:
        return std::make_shared<MoveDownCommand>(player_id);

    case STOP_LEFT:
        return std::make_shared<StopMoveLeftCommand>(player_id);
    case STOP_RIGHT:
        return std::make_shared<StopMoveRightCommand>(player_id);
    case STOP_UP:
        return std::make_shared<StopJumpCommand>(player_id);
    case STOP_DOWN:
        return std::make_shared<StopMoveDownCommand>(player_id);
    case SHOOT:
        return std::make_shared<ShootCommand>(player_id);
    case TAKE_ITEM:
        return std::make_shared<TakeItemCommand>(player_id);
    case DROP_WEAPON:
        return std::make_shared<DropWeaponCommand>(player_id);
    case EXIT_GAME:
        break;
    default:
        break;
    }

    //TODO: manejar el error
    return std::make_shared<MoveLeftCommand>(player_id);

}


void ServerProtocol::shutdown() {
    skt.shutdown(2);
    skt.close();
}

