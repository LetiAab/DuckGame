#ifndef MATCH_H
#define MATCH_H

#include "player.h"
#include "common/lobby_message.h"
#include "common/queue.h"
#include "common/command.h"
//#include "game.h"
#include <list>

class Match {
    
private:
    uint16_t match_id;
    bool is_running;
    uint8_t min_players;
    uint8_t max_players;
    std::list<std::shared_ptr<Player>> players;
    Queue<Command> queue; //quizas la deba tener game
    //Game game;



public:
    Match(uint16_t match_id);

    bool add_player(std::shared_ptr<Player> player);

    void notify_players(const LobbyMessage& msg);

    void start_match(const LobbyMessage& msg);

    bool is_able_to_start();

    uint16_t get_match_id();

    bool is_match_avaiable();


};

#endif
