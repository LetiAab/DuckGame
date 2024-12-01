#include "common/constants.h"
#include "common/message.h"

#ifndef ROUND_MANAGER_H
#define ROUND_MANAGER_H

#include <vector>

typedef struct DuckRegister
{
    char duck_id;
    int rounds_won;
    int kills;
    int deads;

} DuckRegister;


class RoundManager {

private:
    int rounds;
    char duck_winner;
    char last_round_winner;
    std::vector<DuckRegister> ducks;

public:
    RoundManager();

    void initialize_manager(int ducks_size);

    void declare_round_winner(char duck_id);

    int check_match_status();

    bool check_end_of_five_rounds();

    bool check_end_of_match();

    char get_duck_winner();

    char get_duck_round_winner();

    int get_round();

    bool get_five_round_message(Message &msg);

    bool get_end_round_message(Message &msg);

    bool get_end_match_message(Message &msg);


};

#endif
