#include "match.h"


Match::Match(uint16_t match_id): 
match_id(match_id), is_running(false), min_players(2), max_players(6), current_players(0), players(),
monitor(), game(match_id, monitor) {}


bool Match::add_player(std::shared_ptr<Player> player) {
    players.push_back(player);
    return true;

}

bool Match::add_player() {
    if(is_match_available()){
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

    std::vector<uint16_t> player_ids; //este auxiliar para mandarle al inicializador de patos
    
    for(auto& player: players){
        player_ids.push_back(player->get_player_id());
        monitor.add_queue(&player->get_message_queue());
        player->start_playing();
    }

    //cuando inicio el match tengo que crear a los patos dentro de la lista de patos del game
    
    game.map.setEscenario();
    game.create_ducks(player_ids);
    game.start();
}

uint16_t Match::get_match_id() {
    return match_id;
}

bool Match::is_match_available() {
    if ((current_players < max_players) and !is_running) {
        return true;
    }
    return false;
}

Queue<std::shared_ptr<Executable>>& Match::get_game_queue(){
    return game.get_game_queue();
}
