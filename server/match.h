#ifndef MATCH_H
#define MATCH_H

#include "player.h"
#include <list>

class Match {
    
private:
    int match_id;
    bool is_running;
    uint8_t min_players;
    uint8_t max_players;
    std::list<Player*> players;
    //Game game;



public:
    Match(int match_id);

    bool add_player(Player* player);

    bool start_match();

    int get_match_id();


};

#endif
