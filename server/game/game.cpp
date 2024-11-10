#include "game.h"
#include "common/constants.h"

#include <random>
#include <utility>

//TODO: Tamanio del mapa hardcodeado
Game::Game(uint16_t match_id, GameQueueMonitor& monitor):
match_id(match_id), monitor(monitor), is_running(true), game_queue(), map(MATRIX_M, MATRIX_N){}

Queue<std::shared_ptr<Executable>>& Game::get_game_queue(){
        return game_queue;
}



void Game::simulate_round() {
        
    for (Duck& duck : ducks) {  
        duck.update_position_speed();
        
        for (auto it = duck.bullets.begin(); it != duck.bullets.end(); ) {
                it->update_position();
                
                if (it->hubo_impacto()) {
                        it->cleanPostImpacto();
                        it = duck.bullets.erase(it);
                } else {
                ++it;
                }
        }
    }

    for (std::unique_ptr<Proyectil>& projectile : projectiles) {
        projectile->simular(*this);  // Llama a la función simular específica de cada proyectil
    }
}

void Game::add_projectile(std::unique_ptr<Proyectil> projectile) {
    // Transfiere la propiedad del proyectil usando std::move
    projectiles.push_back(std::move(projectile));

}

void Game::sendDuckPositionUpdate(const Duck& duck) {
    std::cout << "Envío el mensaje con la posición x: " << duck.get_x() 
              << " y: " << duck.get_y() << std::endl;

    Message message;
    message.player_id = static_cast<uint16_t>(duck.get_id() - '0'); // Convertimos el id a int
    message.type = DUCK_POS_UPDATE;
    message.duck_x = duck.get_x();
    message.duck_y = duck.get_y();
    message.looking = duck.looking;
    message.is_moving = duck.is_moving;  // Indicamos si el pato está en movimiento o no
    monitor.broadcast(message);
}

void Game::sendBulletPositionUpdate(const Bullet& bullet) {

    Message message;
    message.type = BULLET_POS_UPDATE;
    message.bullet_x = bullet.get_x();
    message.bullet_y = bullet.get_y();
    monitor.broadcast(message);
}



void Game::run() {

        //Creo el mapa con los objetos fijos (bloques) y la posicion inicial de los patos
        inicializate_map();
        //map.printMap();


        //Mando la posicion de todo el mapa por primera vez para tener referencia de donde estan
        //Todos los obstaculos
        Message message;
        message.type = MAP_INICIALIZATION;
        message.map = map.getMap();

        monitor.broadcast(message);

        while (is_running) {
                // saco de 5 comandos de la queue y los ejecuto
                std::shared_ptr<Executable> command;

                int i = 0;
                while( i < 5 && game_queue.try_pop(command)){
                        command->execute(*this);

                        i += 1;
                }

                

                // Simulo una ronda de movimientos
                simulate_round();

                //mando la posicion de cada PATO
                //NO ME GUSTA NADA ESTO PORQUE NO RESPETA QUIEN SE MOVIO PRIMERO
              for (Duck& duck : ducks) {
                
                for (Bullet& bullet : duck.bullets) {
                        
                        sendBulletPositionUpdate(bullet);
                        //bullet.update_position();
                }

                bool is_stationary = (duck.get_x() == duck.get_old_x()) && (duck.get_y() == duck.get_old_y());

                if (is_stationary) {
                        if (!duck.stop_notificated) {
                        duck.is_moving = false;

                        sendDuckPositionUpdate(duck);

                        duck.stop_notificated = true;
                        }
                        continue;
                } else {
                        sendDuckPositionUpdate(duck);
                        duck.stop_notificated = false;

                }

                duck.set_old_x(duck.get_x());
                duck.set_old_y(duck.get_y());
                }


                // renew_iteration(); para resetear cosas que duren una ronda

                std::this_thread::sleep_for(std::chrono::milliseconds(TIME_SLEEP));

        }

}

void Game::inicializate_map(){
/*         
                Things to initialize:
                - Players
                - Boxes
                - Spawn places
                - Stage grounds
                - Map limit (if we model it)
 */
}

//TODO: Esto solo sirve para dos patos y siempre tiene en cuenta que es el mismo distribucion de obstaculos
void Game::create_ducks(const std::vector<uint16_t>& ids) {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib_x(1, map.get_width() - 2);
        std::uniform_int_distribution<> distrib_y(1, map.get_width() - 3);

        for(uint16_t id: ids) {
                char char_id = static_cast<char>(id + '0');

                int random_x = distrib_x(gen);
                int random_y = distrib_y(gen);

                bool has_place = map.placeDuck(random_x, random_y, char_id);
                while (has_place == false) {
                        random_x = distrib_x(gen);
                        random_y = distrib_y(gen);

                        has_place = map.placeDuck(random_x, random_y, char_id); 
                }

                ducks.emplace_back(char_id, random_x, random_y, map);
        }
}


Duck* Game::getDuckById(char id) {
        for (auto& duck : ducks) {
                if (duck.get_id() == id) {
                        return &duck; // Retorna un puntero al pato encontrado
                }
        }
        return nullptr; // Retorna nullptr si no se encuentra el pato
}


void Game::game_broadcast(Message message){
        monitor.broadcast(message);
}

void Game::stop() { is_running = false; }
