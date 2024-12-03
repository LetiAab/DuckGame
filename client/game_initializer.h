#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

#include "game_state.h"
#include "../common/message.h"

class GameInitializer {
public:

    void initialize_new_round(GameState &game, Queue<Message> &message_queue);
    void initializeGame(Queue<Message> &message_queue, GameState& game, int frame_width, int frame_height);

private:

    void createDucks(Queue<Message> &message_queue, GameState &game, const int frame_width, const int frame_height);
    void initializeDucks(GameState &game, Queue<Message> &message_queue);
    void initializeCrates(GameState& game);
    void initializeSpawnPlaces(GameState& game, Queue<Message> &message_queue);
    void initializeBoxes(GameState& game, Queue<Message> &message_queue);
    void initializeMap(GameState& game, Queue<Message> &message_queue);
    void initializeItemsOnFloor(GameState& game, Queue<Message> &message_queue);


};


#endif //GAME_INITIALIZER_H
