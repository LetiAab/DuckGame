#ifndef GAME_H
#define GAME_H

#include "../executables/executable.h"
#include "game_queue_monitor.h"
#include "../../common/queue.h"
#include "../../common/thread.h"
#include "../../common/command.h"

#include "../armor/armor.h"
#include "../armor/helmet.h"
#include "game_map.h"
#include "duck.h"
#include "item.h"
#include "../guns/weapon.h"
#include "spawn_place.h"
#include "box.h"
#include "round_manager.h"
#include "../level_manager.h"

#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

// En gun.h tengo la misma variable, cambiar también ahí o mejorar el import

typedef struct update
{
        int duck_id;
        int previous_x_position;
        int previous_y_position;
        int x_movement;
        int y_movement;
} Update;
// Con esta lógica el cliente tendría que chequear quien está en esa posición y después moverlo


//se me genera una dependencia circular entre game y executable
class Executable;

class Game: public Thread {

private:
        uint16_t match_id;
        GameQueueMonitor& monitor;
        bool& is_over;
        bool is_running;
        RoundManager round_manager;
        Queue<std::shared_ptr<Executable>> game_queue;
        int players; //cantidad de jugadores
        LevelManager level_manager;
        std::vector<Update> updates;

        void disconect_all_players();


        void send_updates();
        void send_throwed_weapon_updates();

        void initialize_round();
        void initialize_ducks(std::vector<Position> ducks_positions);
        Position get_random_position_for_duck(char duck_id);

        void create_ducks(int size, std::vector<Position> ducks_positions);
        void create_items();

        void create_spawn_places(std::vector<Position> spawns_positions);
        void create_boxes(std::vector<Position> boxes_positions);

        void create_items_on_floor(std::vector<ItemConfig> items_on_floor_positions);
        
        void refreshDuckPositions();
        void simulate_round();
        void simulate_throwed_weapons();
        void search_banana_collision();


        bool check_end_game();
        bool check_end_of_round();
        void notify_players_end_game();
        void notify_players_end_round();
        void notify_players_end_of_five_rounds();
        void send_spawn_place_message();
        void send_map_message();
        void send_initialize_ducks_message();
        void send_boxes_initialize_message();
        void send_items_on_floor_message();



public:
        GameMap map;
        std::vector<Duck> ducks;
        std::vector<std::shared_ptr<Item>> items;
        std::vector<std::unique_ptr<SpawnPlace>> spawn_places;
        std::vector<std::shared_ptr<Weapon>> throwed_weapons;
        std::vector<std::unique_ptr<Box>> boxes;
        std::vector<std::shared_ptr<Item>> items_on_floor;



explicit Game(uint16_t match_id, GameQueueMonitor& monitor, bool& is_over);

Queue<std::shared_ptr<Executable>>& get_game_queue();


void set_players(int number_of_players);

//void inicializate_map();

Duck* getDuckByPosition(Position position);

Duck* getDuckById(char id);

std::shared_ptr<Item> getItemByPosition(Position position);

std::shared_ptr<Item> getItemOnFloorByPosition(Position position);


SpawnPlace* getSpawnPlaceByPosition(Position position);

void add_throwed_weapon(Weapon* throwed_weapon);
Box* getBoxByPosition(Position position);


void game_broadcast(Message message);

//void add_projectile(std::unique_ptr<Proyectil> projectile);

void run() override;

void stop() override;


Game(const Game&) = delete;
Game& operator=(const Game&) = delete;


};

#endif  // GAME_H

