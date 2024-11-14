#include "item.h"

Item::Item(int x, int y) : position(x, y) {}

Position Item::getPosition() const {
    return position;
}

void Item::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

bool Item::getItemPositionMessage(Message& msg){
    //TODO: Esto se tendria que implementar en las hijas àra saner que item id mandar
    msg.type = ITEM_POSITION;
    msg.item_x = position.x;
    msg.item_y = position.y;
    msg.item_id = WEAPON_1_ID;

    return true;

}