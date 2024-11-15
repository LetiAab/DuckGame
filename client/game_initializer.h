#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

#include "game_state.h"

class GameInitializer {
public:
    void initializeDucks(GameState* game, int frame_width, int frame_height);
    void initializeCrates(GameState* game);

};


#endif //GAME_INITIALIZER_H
