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

       //map.printMap();

    for (Duck& duck : ducks) {  
        duck.update_position();
        //revisar: es raro que el chaequeo del impacto se haga aca, quizas deberia hacerlo el arma (?)
        if(duck.weapon != nullptr){
                for (auto it = duck.weapon->bullets.begin(); it != duck.weapon->bullets.end(); ) {
                        it->update_position();
                        
                        if (it->hubo_impacto()) {
                                //cuando la bala impacta la saco de la lista de lanzadas por el pato
                                //de esta forma libero y encima me ahorro mandar el mensaje de la pos de la bala una vez que no existe mas. Porque el mensaje se crea viendo la lista de balas. Cosa que no me gusta demasiado pero bueno
                                Position bullet_pos = it->get_position();

                                std::cout << "HUBO IMPACTO en x: " << bullet_pos.x << " y:" << bullet_pos.y << std::endl;
                                std::cout << "La longitud del vector de bullets es " << duck.weapon->bullets.size() << std::endl;

                                Duck* duck_hit = getDuckByPosition(bullet_pos);
                                if (duck_hit == NULL) {
                                        it->cleanPostImpacto();
                                        it = duck.weapon->bullets.erase(it);
                                        continue;
                                }

                                duck_hit->get_hit_by_bullet(*it);

                                it->cleanPostImpacto();
                                it = duck.weapon->bullets.erase(it);
                        } else {
                                ++it;
                        }
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

                        Message duck_message;
                        if(duck.get_duck_position_message(duck_message)){
                                monitor.broadcast(duck_message);
                        }

                        //quizas se pueda hacer un get_duck_bullet_position() en vez de esto
                        //o sea mover la creacion del mensaje dentro del pato
                        if(duck.weapon != nullptr){
                                for (Bullet& bullet : duck.weapon->bullets) {
                                        Message bullet_message;
                                        //por ahora este if da siempre true, hay que agregar logica 
                                        //en el get_bullet_message para que se envia el mensaje solo cuando ees necessario
                                        if (bullet.get_bullet_message(bullet_message)){
                                                monitor.broadcast(bullet_message);
                                        }
                                }
                        }
                }


                // renew_iteration(); para resetear cosas que duren una ronda

                std::this_thread::sleep_for(std::chrono::milliseconds(GAME_TIME_SLEEP));

        }

}

void Game::inicializate_map() {
    // Le doy armas a los patos para probar
    for (Duck& duck : ducks) {

        Weapon* weapon = new Weapon("Pistola Generica", 10, 5, 30);

        duck.setWeapon(weapon);
    }
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
