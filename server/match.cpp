#include "match.h"
#include "common/constants.h"

Match::Match(uint16_t match_id): 
match_id(match_id), is_running(false), over(false), min_players(MIN_PLAYERS), max_players(MAX_PLAYERS), current_players(0), players(),
monitor(), game(match_id, monitor, over) {}

bool Match::add_player(std::shared_ptr<Player> player) {
    players.push_back(player);
    return true;
}

bool Match::can_add_player() {
    if(is_match_available()){
        current_players += 1;
        return true;
    }
    return false;
}

void Match::remove_player(){
    if (current_players > 0) {
        current_players -= 1;
    }
}

bool Match::is_able_to_start(){
	if ((current_players >= min_players) and !is_running) {
        return true;
    }
    return false;
}

void Match::start_match() {


    //envio a cada jugador el id de su pato, el cual se usara durante todo el juego
    //quizas tambien deberia mandarle su color (?)
    uint16_t duck_id = 1;
    for(auto& player: players){
        Message msg;
        msg.type = FIRST_GAME_MESSAGE;
        msg.player_id = duck_id;
        player->get_message_queue().push(msg);
        player->set_player_id(duck_id);
        duck_id += 1;
    }


    for(auto& player: players){
        monitor.add_queue(&player->get_message_queue());
        player->start_playing();
    }

    //cuando inicio el match tengo que crear a los patos dentro de la lista de patos del game

    game.set_players(players.size());
    game.start();

    is_running = true;
}

void Match::stop_match(){
    if(is_running){
        game.stop();
        game.join();
        std::cout << "Match: joinie game"  << std::endl;
        for(auto& player: players){
            player->stop_playing();
        }
        
        players.clear();
        std::cout << "Match: elimine a los players de mi partida"  << std::endl;
    }
}

bool Match::is_over(){
    return over;
}

uint16_t Match::get_match_id() {
    return match_id;
}

bool Match::is_match_available() {
    if ((current_players <= max_players) and !is_running) {
        return true;
    }
    return false;
}

Queue<std::shared_ptr<Executable>>& Match::get_game_queue(){
    return game.get_game_queue();
}
