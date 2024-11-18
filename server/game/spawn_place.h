#ifndef SPAWN_PLACE_H
#define SPAWN_PLACE_H

#include "item.h"
#include "common/position.h" 

class SpawnPlace {
private:
    Position position;    
    int id;          
    uint16_t item_id;  

public:
    SpawnPlace(const Position& position, int id, uint16_t item_id = 0);

    const Position& getPosition() const; 
    void setPosition(const Position& newPosition); 

    int getId() const;
    void setId(int newId); 

    bool getSpawnPlacePositionMessage(Message& msg);

};

#endif // SPAWN_PLACE_H
