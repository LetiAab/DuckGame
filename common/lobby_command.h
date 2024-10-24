#ifndef LOBBY_COMMAND_H
#define LOBBY_COMMAND_H

#include <cstdint>
#include <string>

struct LobbyCommand {
    uint16_t player_id; //probablemente no sea seguro mandar el id asi
    uint8_t type;
    uint16_t match_id;
};

#endif
