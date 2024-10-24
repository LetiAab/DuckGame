#ifndef MATCH_H
#define MATCH_H

#include "player.h"
#include <list>

class Match {
    
private:
    uint16_t match_id;
    bool is_running;
    uint8_t min_players;
    uint8_t max_players;
    std::list<std::shared_ptr<Player>> players;
    //Game game;



public:
    Match(uint16_t match_id);

    bool add_player(std::shared_ptr<Player> player);

    bool start_match();

    uint16_t get_match_id();

    bool is_match_avaiable();


};

#endif
