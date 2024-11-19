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

struct EndGame: public std::runtime_error {
    EndGame(): std::runtime_error("END GAME!") {}
};

class Game: public Thread {

private:
        uint16_t match_id;
        GameQueueMonitor& monitor;
        bool& is_over;
        bool is_running;
        Queue<std::shared_ptr<Executable>> game_queue;
        std::vector<Update> updates;
        //std::vector<std::unique_ptr<Proyectil>> projectiles;

public:
        GameMap map;
        std::vector<Duck> ducks;
        std::vector<std::shared_ptr<Item>> items;
        std::vector<std::unique_ptr<SpawnPlace>> spawn_places;



explicit Game(uint16_t match_id, GameQueueMonitor& monitor, bool& is_over);

Queue<std::shared_ptr<Executable>>& get_game_queue();

void inicializate_map();

void create_ducks(int size);

void create_spawn_places();

void refreshDuckPositions();

Duck* getDuckByPosition(Position position);

Duck* getDuckById(char id);

std::shared_ptr<Item> getItemByPosition(Position position);

SpawnPlace* getSpawnPlaceByPosition(Position position);


void game_broadcast(Message message);

void simulate_round();

//void add_projectile(std::unique_ptr<Proyectil> projectile);

bool check_end_game();

void run() override;

void stop() override;

void notify_players_end_game();

Game(const Game&) = delete;
Game& operator=(const Game&) = delete;
};

#endif  // GAME_H

