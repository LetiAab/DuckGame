#ifndef GAME_H
#define GAME_H

#include "game_queue_monitor.h"
#include "executables/executable.h"
#include "common/queue.h"
#include "common/thread.h"
#include "common/command.h"
#include "game_map.h"
#include <list>
#include <vector>


class Game: public Thread {

private:
        uint16_t match_id;
        GameQueueMonitor& monitor;
        bool is_running;
        Queue<std::shared_ptr<Executable>> game_queue;
        GameMap map;

public:
explicit Game(uint16_t match_id, GameQueueMonitor& monitor);

Queue<std::shared_ptr<Executable>>& get_game_queue();

void run() override;
void stop() override;

Game(const Game&) = delete;
Game& operator=(const Game&) = delete;
};





/*
enum update_type {
        Move, Jump, Shoot, TakeGunOrArmor, LeaveGun, LeaveArmor, AimUp, Floor
};

typedef struct update {
        update_type type;
        int player_id;
        // More fields?
} Update;

typedef struct duck {
        int duck_id;            // Maybe a name instead
        int x_position;
        int y_position;
        int equipped_weapon;    // Should be an object
        int equipped_armor;     // Should be an object
        // bool is_fluttering;
        // More fields?
} Duck;

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
        

        void initialize_map(Update update);

        void check_move_effects(Duck moving_duck);

        bool check_valid_position(int duck_x_position, int duck_y_position);

        void duck_move(int player_id);

        bool is_touching_floor(int duck_x_position, int duck_y_position);

        void duck_jump(int player_id);

        void duck_shoot(int player_id);

        void duck_take_item(int player_id);

        void duck_drop_gun(int player_id);

        void duck_drop_armor(int player_id);

        void duck_aim_up(int player_id);

        void duck_floor(int player_id);

        void process_update(Update update);

        void broadcast_changes(Update update);

        void renew_iteration(Update update);
*/



#endif  // GAME_H
