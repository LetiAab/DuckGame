#include "round_manager.h"
#include <cstdlib>
#include <iostream>

RoundManager::RoundManager(): rounds(1), duck_winner('0'), last_round_winner('0') {}

void RoundManager::initialize_manager(int ducks_size) {
    ducks.clear();
    ducks.reserve(ducks_size);
    for (int i = 1; i <= ducks_size; ++i) {
        char char_id = static_cast<char>(i + '0');
        ducks.push_back({char_id, 0, 0, 0});
    }
}

void RoundManager::declare_round_winner(char duck_id) {

    last_round_winner = duck_id;
    for (auto& duck : ducks) {
        if (duck.duck_id == duck_id) {
            std::cout << "El ganador de la ronda "<< rounds << " fue el pato "<< static_cast<char>(duck_id) << std::endl;
            duck.rounds_won += 1;
            return;
        }
    }

}

int RoundManager::get_round(){return rounds;}

int RoundManager::check_match_status(){
    if(check_end_of_match()){
        return MATCH_HAS_WINNER;
    }

    if(check_end_of_five_rounds()){
        //pasaron 5 rondas pero no hay ganador aun
        rounds += 1;
        std::cout << "Empieza la ronda " << rounds <<  std::endl;
        return MATCH_5_ROUNDS;
    }

    rounds += 1;
    std::cout << "Empieza la ronda " << rounds <<  std::endl;
    return MATCH_NEXT_ROUND;
    
}

bool RoundManager::check_end_of_five_rounds(){
    return (rounds % 5 == 0);
}


bool RoundManager::check_end_of_match() {
    if (!check_end_of_five_rounds()){
        return false;
    }

    int max_victories = 0;
    char winner = '0';
    bool tie = false;

    for (const auto& duck : ducks) {
        if (duck.rounds_won >= 10) {
            if (duck.rounds_won > max_victories) {
                max_victories = duck.rounds_won;
                winner = duck.duck_id;
                tie = false;
            } else if (duck.rounds_won == max_victories) {
                tie = true;
            }
        }
    }

    if (tie || max_victories < 10) {
        return false;
    }

    duck_winner = winner;
    return true;
}

char RoundManager::get_duck_winner(){
    return duck_winner;
}

char RoundManager::get_duck_round_winner(){
    return last_round_winner;
}

bool RoundManager::get_five_round_message(Message &msg){
    msg.type = END_FIVE_ROUNDS;
    msg.round = rounds;
    msg.duck_winner = static_cast<uint16_t>(last_round_winner - '0');
    msg.ducks_quantity = ducks.size();
    
    for(auto &duck: ducks){
        msg.scoreboard.push_back(duck.rounds_won);
    }
    return true;
}

bool RoundManager::get_end_round_message(Message &msg){
    msg.type = END_ROUND;
    msg.round = rounds;
    msg.duck_winner = static_cast<uint16_t>(last_round_winner - '0');
    return true;
    
}

bool RoundManager::get_end_match_message(Message &msg){
    msg.type = END_GAME;
    msg.round = rounds;
    msg.duck_winner = static_cast<uint16_t>(duck_winner - '0');
    msg.ducks_quantity = ducks.size();
    
    for(auto &duck: ducks){
        msg.scoreboard.push_back(duck.rounds_won);
    }

    return true;

}