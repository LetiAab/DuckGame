#include "game.h"
#include "common/constants.h"

#include <random>
#include <utility>

const char DUCK_1 = '1';
const char DUCK_2 = '2';
const char DUCK_3 = '3';
const char DUCK_4 = '4';
const char DUCK_5 = '5';
const char DUCK_6 = '6';

const int NUM_ITEMS = 10;

//TODO: Tamanio del mapa hardcodeado
Game::Game(uint16_t match_id, GameQueueMonitor& monitor):
match_id(match_id), monitor(monitor), is_running(true), game_queue(), map(MATRIX_M, MATRIX_N){}

Queue<std::shared_ptr<Executable>>& Game::get_game_queue(){
        return game_queue;
}

Duck* Game::getDuckByPosition(Position position) {
        char element = map.at(position);

        if ((element == DUCK_1) || (element == DUCK_2) || (element == DUCK_3)
             || (element == DUCK_4) || (element == DUCK_5) || (element == DUCK_6)) {
                return getDuckById(element);
        } else {
                return NULL;
        }
}

void Game::simulate_round() {

        //map.printMap();

        for (auto it = ducks.begin(); it != ducks.end(); ) {  
                Duck& duck = *it;
                duck.update_life();
                duck.update_position();
                
                if (duck.weapon != nullptr) {
                        for (auto bullet_it = duck.weapon->bullets.begin(); bullet_it != duck.weapon->bullets.end(); ) {
                                bullet_it->update_position();
                                
                                if (bullet_it->hubo_impacto()) {
                                        bullet_it->cleanPostImpacto();
                                        bullet_it = duck.weapon->bullets.erase(bullet_it);
                                        
                                } else {
                                        ++bullet_it;
                                }
                        }
                }

                // Si el pato está muerto, lo eliminamos de la lista y avisamos al cliente
                if (duck.is_dead) {
                        Message kill_duck_message;
                        kill_duck_message.type = KILL_DUCK;
                        kill_duck_message.player_id = static_cast<uint16_t>(duck.get_id() - '0');
                        monitor.broadcast(kill_duck_message);
                        it = ducks.erase(it);

                        std::cout << "Pato eliminado. Tamaño actual de ducks: " << ducks.size() << std::endl;
                } else {
                        ++it;
                }
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


        //creo los items y le mando al server
        create_items();


        while (is_running) {
                // saco de 10 comandos de la queue y los ejecuto
                std::shared_ptr<Executable> command;

                int i = 0;
                while( i < 10 && game_queue.try_pop(command)){
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

                std::this_thread::sleep_for(std::chrono::milliseconds(60));

        }

}

void Game::inicializate_map() {
    // Le doy armas a los patos para probar
    
    for (Duck& duck : ducks) {

        Weapon* weapon = new Weapon("Pistola Generica", 35, 5, 30);

        duck.setWeapon(weapon);
    }
}

//TODO: Esto solo sirve para dos patos y siempre tiene en cuenta que es el mismo distribucion de obstaculos
void Game::create_ducks(const std::vector<uint16_t>& ids) {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib_x(18, map.get_width() - 18);
        std::uniform_int_distribution<> distrib_y(10, map.get_height() - 20);

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

                ducks.emplace_back(char_id, random_x, random_y, &map);
        }
}

void Game::create_items() {

    std::srand(static_cast<unsigned>(std::time(nullptr))); // Inicializar la semilla aleatoria

    for (int i = 0; i < 1; ++i) {
        int x = 30;//std::rand() % map.get_width();  // Generar posición aleatoria en el mapa
        int y = 125;//std::rand() % map.get_height();

        // Crear un tipo de ítem aleatorio
        int item_type = std::rand() % 3;
        std::unique_ptr<Item> item;

        if (item_type == 0) {
            item = std::make_unique<Weapon>("Nombre", 100.0, 1.5, 30, x, y);
        } else if (item_type == 1) {
            item = std::make_unique<Armor>(x, y);
        } else {
            item = std::make_unique<Helmet>(x, y);
        }

        // Agregar el ítem al vector de ítems 
        //NO NECESITO A LOS ITEMS EN LA MATRIZ DE COLICIONES PORQUE NO COLICIONAN
        //SI YO INTENTO AGARRAR UN ITEM CON "E" INTENTA AGARRAR EL PATO ALGO QUE ESTE EN SU POSICION
        //SI HAY ALGO LO AGARRA SI NO NO. PARA ESTO REVISA LA LISTA DE ITEMS Y BUSCA ALGUNO
        //QUE COINCIDA CON SU POSICION


        //mando al cliente donde se creo el item para que lo renderice

        std::cout << "MANDO MENSAJE" << "\n";    

        Message item_position_message;
        item->getItemPositionMessage(item_position_message);
        monitor.broadcast(item_position_message);
        
        std::cout << "MANDO MENSAJE?" << "\n";    

        //agrego al vector
        items.push_back(std::move(item));

        
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

Item* Game::getItemByPosition(Position position) {
    for (auto& item_ptr : items) {
        if (item_ptr->getPosition().x == position.x && item_ptr->getPosition().y == position.y) {
            return item_ptr.get(); // Retorna el puntero al item encontrado
        }
    }
    return nullptr; // Retorna nullptr si no se encuentra el item
}

void Game::game_broadcast(Message message){
        monitor.broadcast(message);
}

void Game::stop() { is_running = false; }
