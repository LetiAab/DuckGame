#include "box.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor
Box::Box(const Position& position, int id, GameMap* map,  uint16_t item_id)
    : position(position), id(id), map(map), item_id(item_id), destroyed(false) {}

// Getters y setters
const Position& Box::getPosition() const {
    return position;
}

void Box::setPosition(const Position& newPosition) {
    position = newPosition;
}

uint16_t Box::getItemId() const {
    return item_id;
}

void Box::setItemId(uint16_t newItemId) {
    item_id = newItemId;
}

bool Box::isDestroyed() const {
    return destroyed;
}

void Box::destroy(std::vector<std::shared_ptr<Item>>& items) {
    if (destroyed) {
        return;
    }

    createItem(items);
    destroyed = true;
}

bool Box::getBoxMessage(Message& msg) const {
    
    msg.type = BOX_DESTROYED;
    msg.box_id = id;
    msg.item_id = item_id;

    return true;
}

void Box::createItem(std::vector<std::shared_ptr<Item>>& items) {

    std::srand(static_cast<unsigned>(::time(nullptr)));

    int item_type = std::rand() % 12;
    std::shared_ptr<Item> item;

    if (item_type == 0) {
        item = std::make_shared<Banana>(position.x, position.y);
    } else if (item_type == 1) {
        item = std::make_shared<Grenade>(position.x, position.y);
    } else if (item_type == 2) {
        item = std::make_shared<PewPewLaser>(position.x, position.y);
    } else if (item_type == 3) {
        item = std::make_shared<LaserRifle>(position.x, position.y);
    } else if (item_type == 4) {
        item = std::make_shared<Ak47>(position.x, position.y);
    } else if (item_type == 5) {
        item = std::make_shared<DuelPistol>(position.x, position.y);
    } else if (item_type == 6) {
        item = std::make_shared<CowboyPistol>(position.x, position.y);
    } else if (item_type == 7) {
        item = std::make_shared<Magnum>(position.x, position.y);
    } else if (item_type == 8) {
        item = std::make_shared<Shotgun>(position.x, position.y);
    } else if (item_type == 9) {
        item = std::make_shared<Sniper>(position.x, position.y);
    } else if (item_type == 10) {
        item = std::make_shared<Armor>(position.x, position.y);
    } else {
        item = std::make_shared<Helmet>(position.x, position.y);
    }

    item_id = item->getItemId();
    items.push_back(item);

}

bool Box::update_life(std::vector<std::shared_ptr<Item>>& items){

    if(map->boxIsOverBullet(position)){
        destroy(items);
        map->removeBox(position);
        return true;
    }

    
    return false;

}

void Box::getBoxPositionMessage(Message& msg){
    msg.type = BOX_POSITION;
    msg.box_id = id;
    msg.item_id = item_id;
    msg.box_x = position.x;
    msg.box_y = position.y;
}
