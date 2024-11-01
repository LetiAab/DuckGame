
#ifndef LOBBY_COMMAND_H
#define LOBBY_COMMAND_H

#include <cstdint>
#include <string>

struct LobbyCommand {
    uint16_t player_id;
    uint8_t type;
    uint16_t match_id;

    LobbyCommand(uint16_t player_id, uint8_t type, uint16_t match_id)
        : player_id(player_id), type(type), match_id(match_id) {}
};

#endif
