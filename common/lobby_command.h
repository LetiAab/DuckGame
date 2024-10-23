#ifndef LOBBY_COMMAND_H
#define LOBBY_COMMAND_H

#include <cstdint>
#include <string>

struct LobbyCommand {
    int player_id; //probablemente no sea seguro mandar el id asi
    uint8_t type;
    uint8_t id_partida;
};

#endif
