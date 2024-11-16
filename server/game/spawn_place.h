#ifndef SPAWN_PLACE_H
#define SPAWN_PLACE_H

#include "item.h"
#include "common/position.h" 

class SpawnPlace {
private:
    Position position;    
    int id;          
    Item* item;

public:
    SpawnPlace(const Position& position, int id, Item* item = nullptr);

    const Position& getPosition() const; 
    void setPosition(const Position& newPosition); 

    Item* getItem() const;
    void setItem(Item* newItem);

    int getId() const;
    void setId(int newId); 

    bool getSpawnPlacePositionMessage(Message& msg);

};

#endif // SPAWN_PLACE_H
