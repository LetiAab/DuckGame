#include "match.h"


Match::Match(uint16_t match_id): match_id(match_id), is_running(false), min_players(2), max_players(6), players(), queue() /*,game(match_id) */{}

bool Match::add_player(std::shared_ptr<Player> player) {
    if ((players.size() < max_players) && !is_running) {
        players.push_back(player); 
        return true;
    }
    return false;
}

//le avisa a los jugadores que la partida comenzó para que envien un mensaje que los desconecte del lobby
void Match::notify_players(const LobbyMessage& msg){

	for(auto& player: players){
		player->send_lobby_message(msg);
		//TODO: antes de empezar hay que guardar la queue del sender en un monitor
		player->start_playing();
	}
}

bool Match::is_able_to_start(){
	if ((players.size() >= min_players) && !is_running) {
        return true;
    }
    return false;
}

void Match::start_match(const LobbyMessage& msg) {
    is_running = true;
    notify_players(msg);
    //game.start();
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
