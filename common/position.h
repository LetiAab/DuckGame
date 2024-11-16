#ifndef POSITION_H
#define POSITION_H

#include <cstdint>
#include <string>
#include <vector>

struct Position {
    int x;
    int y;

    Position(int pos_x, int pos_y): x(pos_x), y(pos_y) {}

    bool is_same_position(Position pos){
        return ((pos.x == x) && (pos.y == y));
    }
};


#endif