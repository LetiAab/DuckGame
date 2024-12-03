#include "item.h"

Item::Item(uint16_t item_id, int x, int y) 
    : item_id(item_id), position(x, y) {}
    
Position Item::getPosition() const {
    return position;
}

void Item::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

bool Item::getItemPositionMessage(Message& msg){
    msg.type = ITEM_POSITION;
    msg.item_x = position.x;
    msg.item_y = position.y;
    msg.item_id = item_id;

    return true;

}

//para avisar que no se renderice mas en el piso
void Item::getItemUpdate(Message& msg){
    msg.type = ITEM_ON_FLOOR_UPDATE;
    msg.item_x = position.x;
    msg.item_y = position.y;
    msg.item_id = item_id;
}
