#include "round_manager.h"

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
    int id = static_cast<int>(duck_id - '0');
    last_round_winner = id;
    for (auto& duck : ducks) {
        if (duck.duck_id == id) {
            duck.rounds_won += 1;
            rounds += 1;
            return;
        }
    }

}


bool RoundManager::check_end_of_match() {
    //verifico que pasaron 5 rondas
    if (rounds % 5 == 0){return false;}

    int ducks_with_ten_victories = 0;
    char winner = '0';

    for (const auto& duck : ducks) {
        if (duck.rounds_won >= 10) {
            ducks_with_ten_victories++;
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
