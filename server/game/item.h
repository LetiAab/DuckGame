#ifndef ITEM_H
#define ITEM_H

#include "../../common/position.h"
#include <memory>
#include "../../common/constants.h"
#include "../../common/message.h"

class Item {
protected:
    uint16_t item_id;  
    Position position; 

public:
    Item(uint16_t item_id, int x = 0, int y = 0); 
    virtual ~Item() = default;

    uint16_t getItemId() const { return item_id; } 
    Position getPosition() const;                  
    void setPosition(int x, int y);                
    bool getItemPositionMessage(Message& msg);
    void getItemUpdate(Message& msg);      
};

#endif // ITEM_H
