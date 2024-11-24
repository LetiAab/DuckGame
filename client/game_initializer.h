#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

#include "game_state.h"
#include "common/message.h"

class GameInitializer {
public:

    void initialize_new_round(GameState *game, Queue<Message> &message_queue);
    void initializeGame(Queue<Message> &message_queue, GameState* game, int frame_width, int frame_height);
    void initializeDucks(GameState* game, int frame_width, int frame_height);
    void initializeCrates(GameState* game);

};


#endif //GAME_INITIALIZER_H
