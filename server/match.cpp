#include "match.h"


Match::Match(int match_id): match_id(match_id), is_running(false), min_players(2), max_players(6), players() {}

bool Match::add_player(Player* player){
	if( (players.size() < max_players) and !is_running){
		players.push_back(player);
		return true;
	}
	return false;

}

bool Match::start_match(){
	if((players.size() >= min_players) and !is_running){
		is_running = true;
		//game.start();
		return true;
	}
	return false;
}

int Match::get_match_id(){ return match_id;}


