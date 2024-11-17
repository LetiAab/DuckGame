#include "spawn_place.h"
#include <iostream>

// Constructor
SpawnPlace::SpawnPlace(const Position& position, int id, uint16_t item_id)
    : position(position), id(id), item_id(item_id) {}

// Getters y setters
const Position& SpawnPlace::getPosition() const {
    return position;
}

void SpawnPlace::setPosition(const Position& newPosition) {
    position = newPosition;
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
    msg.item_id = item_id;
    msg.spawn_place_id = id;

    return true;

}