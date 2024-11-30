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
    //verifico que pasaron 5 rondas
    if (!check_end_of_five_rounds()){
        return false;
    }

    int ducks_with_ten_victories = 0;
    char winner = '0';

    for (const auto& duck : ducks) {
        if (duck.rounds_won >= 10) {
            ducks_with_ten_victories++;
            winner = duck.duck_id;
            if (ducks_with_ten_victories > 1) {
                return false; // Hay más de un pato con 10 o más victorias.
            }
        }
    }

    bool result = ducks_with_ten_victories == 1;

    //guardo el id del pato que gano el partido
    if (result){duck_winner = winner; }

    return result;
}

char RoundManager::get_duck_winner(){
    return duck_winner;
}

char RoundManager::get_duck_round_winner(){
    return last_round_winner;
}
