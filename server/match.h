#ifndef MATCH_H
#define MATCH_H

#include "player.h"
#include "executables/executable.h"
#include "../common/queue.h"
#include "../common/command.h"
#include "game/game_queue_monitor.h"
#include "game/game.h"
#include <list>

class Match {
    
private:
    uint16_t match_id;
    bool is_running;
    bool over;
    uint8_t min_players;
    uint8_t max_players;
    uint8_t current_players;
    std::list<std::shared_ptr<Player>> players;
    GameQueueMonitor monitor;
    Game game;



public:
    Match(uint16_t match_id);

    bool add_player(std::shared_ptr<Player> player);

    bool can_add_player();

    void remove_player();

    void start_match();

    void stop_match();

    bool is_over();

    bool is_able_to_start();

    uint16_t get_match_id();

    bool is_match_available();

    Queue<std::shared_ptr<Executable>>& get_game_queue();

    // Testing getters
    int get_players_count();

    bool match_running();


};

#endif
