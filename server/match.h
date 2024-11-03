#ifndef MATCH_H
#define MATCH_H

#include "player.h"
#include "executables/executable.h"
#include "common/queue.h"
#include "common/command.h"
#include "game/game_queue_monitor.h"
#include "game/game.h"
#include <list>

class Match {
    
private:
    uint16_t match_id;
    bool is_running;
    uint8_t min_players;
    uint8_t max_players;
    uint8_t current_players;
    std::list<std::shared_ptr<Player>> players;
    GameQueueMonitor monitor;
    Game game;



public:
    Match(uint16_t match_id);

    bool add_player(std::shared_ptr<Player> player);

    bool add_player();

    void start_match();

    bool is_able_to_start();

    uint16_t get_match_id();

    bool is_match_available();

    Queue<std::shared_ptr<Executable>>& get_game_queue();


};

#endif
