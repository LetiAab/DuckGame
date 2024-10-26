#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>
#include <string>

struct Command {
    uint16_t player_id; //probablemente no sea seguro mandar el id asi
    uint8_t type;

};

#endif
