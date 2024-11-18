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

    bool check_end_of_match();

    char get_duck_winner();

    char get_duck_round_winner();


};

#endif
