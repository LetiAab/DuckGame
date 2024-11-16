#include "spawn_place.h"
#include <iostream>

// Constructor
SpawnPlace::SpawnPlace(const Position& position, int id, Item* item)
    : position(position), id(id), item(item) {}

// Getters y setters
const Position& SpawnPlace::getPosition() const {
    return position;
}

void SpawnPlace::setPosition(const Position& newPosition) {
    position = newPosition;
}

Item* SpawnPlace::getItem() const {
    return item;
}

void SpawnPlace::setItem(Item* newItem) {
    item = newItem;
}

int SpawnPlace::getId() const {
    return id;
}

void SpawnPlace::setId(int newId) {
    id = newId;
}

bool SpawnPlace::getSpawnPlacePositionMessage(Message& msg){
    //TODO: Esto se tendria que implementar en las hijas àra saner que item id mandar
    std::cout << "LE PASO EL SPAWN CON x: " << position.x << " y: " << position.y << "\n";

    msg.type = SPAWN_PLACE_POSITION;
    msg.spaw_place_x = position.x;
    msg.spaw_place_y = position.y;
    msg.item_id = item != nullptr ? item->getItemId() : 0;
    msg.spawn_place_id = id;

    return true;

}