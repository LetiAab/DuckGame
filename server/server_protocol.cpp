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
        throw LibError(errno, CLOSED_SOCKET);
    }
    
    if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
        throw LibError(errno, CLOSED_SOCKET);
    }

    if (!skt.sendall(&message.len_matches, sizeof(message.len_matches), &was_closed) || was_closed) {
        throw LibError(errno, CLOSED_SOCKET);
    }

    for (uint16_t match_id : message.existing_matches) {
        if (!skt.sendall(&match_id, sizeof(match_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
    }

    if (!skt.sendall(&message.current_match_id, sizeof(message.current_match_id), &was_closed) || was_closed) {
        throw LibError(errno, CLOSED_SOCKET);
    }

    return true;
}

bool ServerProtocol::send_message(Message& message){
    bool was_closed = false;

    //mando el type primero siempre
    if (!skt.sendall(&message.type, sizeof(message.type), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

    switch (message.type)
    {
    case END_GAME:
        if (!skt.sendall(&message.round, sizeof(message.round), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.duck_winner, sizeof(message.duck_winner), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.ducks_quantity, sizeof(message.ducks_quantity), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(message.scoreboard.data(), message.ducks_quantity * sizeof(int), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case END_ROUND:
        if (!skt.sendall(&message.round, sizeof(message.round), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.duck_winner, sizeof(message.duck_winner), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case END_FIVE_ROUNDS:
        if (!skt.sendall(&message.round, sizeof(message.round), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.duck_winner, sizeof(message.duck_winner), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.ducks_quantity, sizeof(message.ducks_quantity), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(message.scoreboard.data(), message.ducks_quantity * sizeof(int), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case FIRST_GAME_MESSAGE:
        //le envio el NUEVO ID, que usara en la partida
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case MAP_INICIALIZATION:
        // Enviar la matriz fila por fila
        for (size_t i = 0; i < MATRIX_N; ++i) {
            if (!skt.sendall(message.map[i].data(), MATRIX_M * sizeof(char), &was_closed) || was_closed) {
                throw LibError(errno, CLOSED_SOCKET);
            }
        }

        break;
    
    case SPAWN_PLACES_INICIALIZATION:
        if (!skt.sendall(&message.spawn_places_quantity, sizeof(message.spawn_places_quantity), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;

    case BOXES_INICIALIZATION:
        if (!skt.sendall(&message.boxes_quantity, sizeof(message.boxes_quantity), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;

    case ITEMS_ON_FLOOR_INICIALIZATION:
        if (!skt.sendall(&message.items_on_floor_quantity, sizeof(message.items_on_floor_quantity), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;


    case DUCKS_INICIALIZATION:
        // Enviar la cantidad de patos
        if (!skt.sendall(&message.ducks_quantity, sizeof(message.ducks_quantity), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;

    case DUCK_PICKUP_ITEM:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case DUCK_EQUIP_ITEM:

        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case BOX_POSITION:

        if (!skt.sendall(&message.box_id, sizeof(message.box_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.box_x, sizeof(message.box_x), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.box_y, sizeof(message.box_y), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;

    case BOX_DESTROYED:
        if (!skt.sendall(&message.box_id, sizeof(message.box_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    
    case DROP_WEAPON:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;
    
    case ARMOR_BROKEN:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case HELMET_BROKEN:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    case SHOOT:
        std::cout << "sale el mensaje de shoot \n";
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;
    case ITEM_ON_FLOOR_UPDATE:
    case ITEM_POSITION:
        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_x, sizeof(message.item_x), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_y, sizeof(message.item_y), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;



    case SPAWN_PLACE_ITEM_UPDATE:
        if (!skt.sendall(&message.spawn_place_id, sizeof(message.spawn_place_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;

    
    case SPAWN_PLACE_POSITION:
 
        if (!skt.sendall(&message.spawn_place_id, sizeof(message.spawn_place_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.spaw_place_x, sizeof(message.spaw_place_x), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.spaw_place_y, sizeof(message.spaw_place_y), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.item_id, sizeof(message.item_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        break;

    case THROWABLE_ITEM:
 
        if (!skt.sendall(&message.item_id, sizeof(message.spawn_place_id), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_x, sizeof(message.item_x), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_y, sizeof(message.item_y), &was_closed) || was_closed) {
            return false;
        }

        if (!skt.sendall(&message.item_used, sizeof(message.item_used), &was_closed) || was_closed) {
            return false;
        }    

        if (!skt.sendall(&message.item_touching_floor, sizeof(message.item_touching_floor), &was_closed) || was_closed) {
            return false;
        }    

        break;

    case BULLET_POS_UPDATE:

        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.bullet_x, sizeof(message.bullet_x), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.bullet_y, sizeof(message.bullet_y), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.bullet_id, sizeof(message.bullet_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.bullet_type, sizeof(message.bullet_type), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.bullet_horizontal, sizeof(message.bullet_horizontal), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break; 

    case KILL_DUCK:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        break;


    case DUCK_POS_UPDATE:
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.duck_x, sizeof(message.duck_x), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.duck_y, sizeof(message.duck_y), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.looking, sizeof(message.looking), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.is_moving, sizeof(message.is_moving), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.is_jumping, sizeof(message.is_jumping), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

        if (!skt.sendall(&message.is_fluttering, sizeof(message.is_fluttering), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        if (!skt.sendall(&message.is_laying_down, sizeof(message.is_laying_down), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }
        if (!skt.sendall(&message.is_looking_up, sizeof(message.is_looking_up), &was_closed) || was_closed) {
            throw LibError(errno, CLOSED_SOCKET);
        }

    default:
        break;
    }

    return true;
}


LobbyCommand ServerProtocol::get_lobby_command() {
    uint16_t player_id = 0;
    uint8_t type = 0;
    uint16_t match_id = 0;
    bool was_closed = false;

    skt.recvall(&type, sizeof(type), &was_closed);
    if (was_closed) throw LibError(errno, CLOSED_SOCKET);

    skt.recvall(&player_id, sizeof(player_id), &was_closed);
    if (was_closed) throw LibError(errno, CLOSED_SOCKET);

    skt.recvall(&match_id, sizeof(match_id), &was_closed);
    if (was_closed) throw LibError(errno, CLOSED_SOCKET);

    LobbyCommand cmd(player_id, type, match_id);

    return cmd;
}



std::shared_ptr<Executable> ServerProtocol::receive_command(){

    uint16_t player_id = 0;
    uint8_t type = 0;
    bool was_closed = false;

    skt.recvall(&type, sizeof(type), &was_closed);
    if (was_closed) throw LibError(errno, CLOSED_SOCKET);
    
    skt.recvall(&player_id, sizeof(player_id), &was_closed);
    if (was_closed) throw LibError(errno, CLOSED_SOCKET);
    
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
    case LOOK_UP:
        return std::make_shared<LookUpCommand>(player_id);
    case STOP_LOOK_UP:
        return std::make_shared<StopLookUpCommand>(player_id);


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

