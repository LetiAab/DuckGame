#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

struct Message {
    uint8_t type;
    uint16_t player_id;
    uint16_t len_matches;
    std::vector<uint16_t> existing_matches;
    uint8_t current_match_id;
    std::vector<std::vector<char>> map;

    int duck_x;
    int duck_y;
    char looking; //direccion en la que mira el pato



};


#endif
