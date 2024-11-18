#include "spawn_place.h"
#include <iostream>

// Constructor
SpawnPlace::SpawnPlace(const Position& position, int id, uint16_t item_id, int iterations_to_generate_item)
    : position(position), id(id), item_id(item_id), iterations_to_generate_item(iterations_to_generate_item) {}

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

void SpawnPlace::setItemId(int newItemId) {
    item_id = newItemId;
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

bool SpawnPlace::getSpawnPlaceItemUpdateMessage(Message& msg){
        msg.type = SPAWN_PLACE_ITEM_UPDATE;
        msg.spawn_place_id = id;
        msg.item_id = item_id;
        return true;
}

bool SpawnPlace::updateIterations(std::vector<std::unique_ptr<Item>>& items){

    if(item_id == 0){
        iterations_to_generate_item--;
    }

    if(iterations_to_generate_item == 0){
        //crear un item en la pos del spawn place y encolarlo en la lista de items
        create_items(items);

        iterations_to_generate_item = ITERATIONS_TO_GENERATE_ITEMS;
        return true;
    }

    return false;

}


void SpawnPlace::create_items(std::vector<std::unique_ptr<Item>>& items) {

    std::srand(static_cast<unsigned>(::time(nullptr)));
    
    for (int i = 0; i < 1; ++i) {
        int x = position.x;
        int y = position.y;

        // Crear un tipo de ítem aleatorio
        int item_type = std::rand() % 3;
        std::unique_ptr<Item> item;

        if (item_type == 0) {
            item = std::make_unique<Weapon>("Nombre", 100.0, 1.5, 30, x, y);
        } else if (item_type == 1) {
            item = std::make_unique<Armor>(x, y);
        } else {
            item = std::make_unique<Helmet>(x, y);
        }

        //agrego al vector
        item_id = item->getItemId();

        items.push_back(std::move(item));

    }
}
