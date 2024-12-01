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
    bool is_moving;
    bool is_jumping;
    bool is_fluttering;
    bool is_laying_down;
    bool is_looking_up;

    int bullet_x;
    int bullet_y;
    int bullet_id;
    int bullet_type;
    bool bullet_horizontal;

    int item_x;
    int item_y;
    uint8_t item_id;
    bool item_used;
    bool item_touching_floor;

    int spaw_place_x;
    int spaw_place_y;
    uint8_t spawn_place_id;

    
    uint8_t box_id;
    int box_x;
    int box_y;

    int round;
    uint16_t duck_winner;
    int ducks_quantity;
    std::vector<int> scoreboard;

    int spawn_places_quantity;
    int boxes_quantity;
    int items_on_floor_quantity;



};


#endif
