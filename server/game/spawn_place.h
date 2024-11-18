#ifndef SPAWN_PLACE_H
#define SPAWN_PLACE_H

#include "item.h"
#include "common/position.h" 
#include "../guns/weapon.h"
#include "../armor/armor.h"
#include "../armor/helmet.h"
#include "item.h"

class SpawnPlace {
private:
    Position position;    
    int id;          
    uint16_t item_id;
    int iterations_to_generate_item;

public:
    SpawnPlace(const Position& position, int id, uint16_t item_id = 0, int iterations_to_generate_item = ITERATIONS_TO_GENERATE_ITEMS);

    const Position& getPosition() const; 
    void setPosition(const Position& newPosition); 

    int getId() const;
    void setId(int newId); 

    void setItemId(int newItemId);

    bool getSpawnPlacePositionMessage(Message& msg);

    bool getSpawnPlaceItemUpdateMessage(Message& msg);


    bool updateIterations(std::vector<std::unique_ptr<Item>>& items);
    
    void create_items(std::vector<std::unique_ptr<Item>>& items);


};

#endif // SPAWN_PLACE_H
