#include "match.h"


Match::Match(uint16_t match_id): 
match_id(match_id), is_running(false), min_players(2), max_players(6), current_players(0), players(), queue(), monitor() /*,game(match_id) */{}


bool Match::add_player(std::shared_ptr<Player> player) {
   
    players.push_back(player); 
    return true;

}

bool Match::add_player() {
    if(is_match_avaiable()){
        current_players += 1;
        return true;
    }
    return false;
}


bool Match::is_able_to_start(){
	if ((current_players >= min_players) and !is_running) {
        return true;
    }
    return false;
}

void Match::start_match() {
    is_running = true;
    for(auto& player: players){
        monitor.add_queue(&player->get_message_queue());
        player->start_playing();
    }

    //game.start();
}

uint16_t Match::get_match_id() {
    return match_id;
}

bool Match::is_match_avaiable() {
    if ((current_players < max_players) and !is_running) {
        return true;
    }
    return false;
}

Queue<Command>& Match::get_game_queue(){
    return queue;
}
