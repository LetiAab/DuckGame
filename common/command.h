#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include <string>

struct Command {
    uint16_t player_id; //probablemente no sea seguro mandar el id asi
    uint8_t type;
    uint16_t match_id;

    Command(uint16_t player_id, uint8_t type, uint16_t match_id)
        : player_id(player_id), type(type), match_id(match_id) {}

    Command(uint16_t player_id, uint8_t type)
        : player_id(player_id), type(type), match_id(0) {}

};

#endif
