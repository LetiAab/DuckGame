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

bool ServerProtocol::send_message(Message& message){
    bool was_closed = false;

    switch (message.type)
    {
    case MAP_INICIALIZATION:

        //TODO: Mandar el player id es al pedo. PERO LO PONGO PORQUE NO QUIERO DAR VUELTA TODO
        //LO QUE DEBERIAMOS HACER ES MANDAR SIEMPRE PRIMERO EL TYPE ASI DEL OTRO LADO SABES COMO
        //ESPERAR EL MENSAJE
        if (!skt.sendall(&message.player_id, sizeof(message.player_id), &was_closed) || was_closed) {
            return false;
        }
        
        if (!skt.sendall(&message.type, sizeof(message.type), &was_closed) || was_closed) {
            return false;
        }

        // Enviar la matriz fila por fila
        for (size_t i = 0; i < 10; ++i) { // 10 filas
            if (!skt.sendall(message.map[i].data(), 15 * sizeof(int), &was_closed) || was_closed) {
                return false;
            }
        }

        break;
    
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

    std::cout << "Comando recibido: " << std::endl;    
    std::cout << "  Player ID: " << player_id << std::endl;
    std::cout << "  Type: " << static_cast<int>(type) << std::endl; // Convertir a int para mostrar el valor de uint8_t
    std::cout << "  Match ID: " << match_id << std::endl;

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

