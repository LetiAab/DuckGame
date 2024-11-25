#ifndef GAME_H
#define GAME_H

#include "../executables/executable.h"
#include "game_queue_monitor.h"
#include "common/queue.h"
#include "common/thread.h"
#include "common/command.h"

#include "../armor/armor.h"
#include "../armor/helmet.h"
#include "game_map.h"
#include "duck.h"
#include "item.h"
#include "../guns/weapon.h"
#include "spawn_place.h"
#include "box.h"
#include "round_manager.h"
#include "../parser.h"

#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>

#define GAME_TIME_SLEEP 100
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
        std::vector<Update> updates;
        //std::vector<std::unique_ptr<Proyectil>> projectiles;


        void send_updates();
        void initialize_round();
        void initialize_ducks();
        Position get_random_position_for_duck(char duck_id);

        void create_ducks(int size);
        void create_ducks(int size, std::vector<Position> ducks_positions);
        void create_items();

        void create_spawn_places();
        void create_spawn_places(std::vector<Position> spawns_positions);
        void refreshDuckPositions();
        void simulate_round();
        bool check_end_game();
        bool check_end_of_round();
        void notify_players_end_game();
        void notify_players_end_round();
        void send_spawn_place_message();
        void send_map_message();
        void send_initialize_ducks_message();
        void send_boxes_initialize_message();


public:
        GameMap map;
        std::vector<Duck> ducks;
        std::vector<std::shared_ptr<Item>> items;
        std::vector<std::unique_ptr<SpawnPlace>> spawn_places;
        std::vector<std::unique_ptr<Box>> boxes;




explicit Game(uint16_t match_id, GameQueueMonitor& monitor, bool& is_over);

Queue<std::shared_ptr<Executable>>& get_game_queue();


void set_players(int number_of_players);

//void inicializate_map();

Duck* getDuckByPosition(Position position);

Duck* getDuckById(char id);

std::shared_ptr<Item> getItemByPosition(Position position);

SpawnPlace* getSpawnPlaceByPosition(Position position);

Box* getBoxByPosition(Position position);


void game_broadcast(Message message);

//void add_projectile(std::unique_ptr<Proyectil> projectile);

void run() override;

void stop() override;


void create_boxes();

Game(const Game&) = delete;
Game& operator=(const Game&) = delete;


};

#endif  // GAME_H

