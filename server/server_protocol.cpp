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
    case MAP_INICIALIZATION:

        // Enviar la matriz fila por fila
        for (size_t i = 0; i < MATRIX_N; ++i) {
            if (!skt.sendall(message.map[i].data(), MATRIX_M * sizeof(char), &was_closed) || was_closed) {
                return false;
            }
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

    case BULLET_POS_UPDATE:
        std::cout << "MANDO LA BULLET" << "\n";

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
        return std::make_shared<StopMoveCommand>(player_id);
    case STOP_RIGHT:
        return std::make_shared<StopMoveCommand>(player_id);
    case STOP_UP:
        return std::make_shared<StopMoveCommand>(player_id);
    case STOP_DOWN:
        return std::make_shared<StopMoveCommand>(player_id);
    case SHOOT:
        return std::make_shared<ShootCommand>(player_id);
    case TAKE_ITEM:
        return std::make_shared<TakeItemCommand>(player_id);
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

