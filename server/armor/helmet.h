#ifndef HELMET_H
#define HELMET_H

#include "../game/item.h"
#include "../../common/constants.h"


class Helmet : public Item {
public:
    Helmet(int x = 0, int y = 0);  // Constructor que inicializa la posición
};

#endif // HELMET_H
