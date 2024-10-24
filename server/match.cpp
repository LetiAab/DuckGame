#include "match.h"


Match::Match(uint16_t match_id): match_id(match_id), is_running(false), min_players(2), max_players(6), players() {}

bool Match::add_player(std::shared_ptr<Player> player) {
    if ((players.size() < max_players) && !is_running) {
        players.push_back(player); 
        return true;
    }
    return false;
}

bool Match::start_match() {
    if ((players.size() >= min_players) && !is_running) {
        is_running = true;
        //game.start();
        return true;
    }
    return false;
}

uint16_t Match::get_match_id() {
    return match_id;
}

bool Match::is_match_avaiable() {
    if ((players.size() < max_players) && !is_running) {
        return true;
    }
    return false;
}