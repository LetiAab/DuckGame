#ifndef GAME_H
#define GAME_H

#include "../executables/executable.h"
#include "game_queue_monitor.h"
#include "common/queue.h"
#include "common/thread.h"
#include "common/command.h"
#include "game_map.h"
#include <list>
#include <vector>
#include "duck.h"

#include <cstdlib>  // Para rand y srand
#include <ctime>    // Para time


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
        bool is_running;
        Queue<std::shared_ptr<Executable>> game_queue;
        std::vector<Update> updates;
        std::vector<std::unique_ptr<Proyectil>> projectiles;

public:
        GameMap map;
        std::vector<Duck> ducks;



explicit Game(uint16_t match_id, GameQueueMonitor& monitor);

Queue<std::shared_ptr<Executable>>& get_game_queue();

void inicializate_map();

void create_ducks(const std::vector<uint16_t>& ids);

void refreshDuckPositions();

Duck* getDuckById(char id);

void game_broadcast(Message message);

void simulate_round();

void add_projectile(std::unique_ptr<Proyectil> projectile);

void sendBulletPositionUpdate(const Bullet& bullet); 

void run() override;
void stop() override;

Game(const Game&) = delete;
Game& operator=(const Game&) = delete;
};

#endif  // GAME_H

