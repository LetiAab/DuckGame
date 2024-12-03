#ifndef BOX_H
#define BOX_H

#include "common/position.h" 
#include "../guns/weapon.h"
#include "../guns/pew_pew_laser.h"
#include "../guns/laser_rifle.h"
#include "../guns/ak_47.h"
#include "../guns/duel_pistol.h"
#include "../guns/cowboy_pistol.h"
#include "../guns/magnum.h"
#include "../guns/shotgun.h"
#include "../guns/sniper.h"
#include "../guns/banana.h"
#include "../guns/grenade.h"
#include "../armor/armor.h"
#include "../armor/helmet.h"
#include "item.h"

#include "game_map.h"
#include <memory>
#include <vector>

class GameMap;

class Box {
private:
    Position position;  
    int id;
    GameMap* map;
    uint16_t item_id;      
    bool destroyed;
    

public:
    Box(const Position& position, int id, GameMap* map,  uint16_t item_id = 0);

    const Position& getPosition() const;
    void setPosition(const Position& newPosition);

    uint16_t getItemId() const;
    void setItemId(uint16_t newItemId);

    bool isDestroyed() const;
    void destroy(std::vector<std::shared_ptr<Item>>& items);

    bool getBoxMessage(Message& msg) const;

    void getBoxPositionMessage(Message& msg);

    bool update_life(std::vector<std::shared_ptr<Item>>& items); //true si se rompio

private:
    void createItem(std::vector<std::shared_ptr<Item>>& items);
};

#endif // BOX_H
