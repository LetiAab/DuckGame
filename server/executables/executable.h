// Interfaz Executable
#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <memory>
#include "../game/game.h"

//tengo una dependencia circular que no me gusta nada
class Game; // Forward declaration

class Executable {
public:
    virtual ~Executable() = default;
    //seguramente reciba un game y un vector de Messages
    virtual void execute(Game& game) = 0; // Cambia aquí
};

#endif

