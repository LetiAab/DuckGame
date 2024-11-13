#include "item.h"

Item::Item(int x, int y) : position(x, y) {}

Position Item::getPosition() const {
    return position;
}

void Item::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}
