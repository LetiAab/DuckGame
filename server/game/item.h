#ifndef ITEM_H
#define ITEM_H

#include "common/position.h"
#include <memory>
#include "common/constants.h"
#include "common/message.h"


class Item {
protected:
    Position position;  // Cada item tendrá una posición

public:
    Item(int x = 0, int y = 0);  // Constructor con posición por defecto
    virtual ~Item() = default;

    Position getPosition() const;  // Método para obtener la posición del item
    void setPosition(int x, int y);  // Método para cambiar la posición del item
    bool getItemPositionMessage(Message& msg);

};

#endif // ITEM_H
