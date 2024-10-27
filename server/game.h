#ifndef GAME_H
#define GAME_H

#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/command.h"
#include <list>
#include <vector>

enum update_type {
        Move, Jump, Shoot, TakeGunOrArmor, LeaveGun, LeaveArmor, AimUp, Floor
};

typedef struct update {
        update_type type;
        int player_id;
        // More fields?
} Update;

#define SLEEP_TIME 200

class Game: public Thread {
private:
    //Monitor& monitor;
    uint16_t match_id;
    bool is_running;
    std::list<std::shared_ptr<Player>> players; // Should be a monitor
    Queue<Command> queue;
    std::vector<std::vector<char>> scenario;
    int game_round;


    bool process_commands();

    void pick_box(int box_id, const char* player_name);

    void simulate_iteration();

public:
    //explicit Game(Monitor& monitor);
    explicit Game(uint16_t match_id);

    void run() override;
    void stop() override;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

#endif  // GAME_H
