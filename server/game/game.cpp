#include "game.h"

#include "common/constants.h"
#include "../guns/sniper.h"
#include "../guns/cowboy_pistol.h"

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
Game::Game(uint16_t match_id, GameQueueMonitor& monitor,  bool& is_over):
match_id(match_id), monitor(monitor), is_over(is_over), is_running(true), game_queue(), map(MATRIX_M, MATRIX_N){}

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

        //sumo 1 al contador de rondas hasta generar un nuevo item en el spawn place
        for (const auto& spawn_place : spawn_places) {
                if(spawn_place->updateIterations(items)){
                        Message msg;
                        spawn_place->getSpawnPlaceItemUpdateMessage(msg);
                        monitor.broadcast(msg);
                }
        }


        for (Duck& duck : ducks) {
                if (duck.is_dead) {
                        // Si el pato murio en la ronda anterior, lo saltamos y continuamos con el siguiente
                        continue;
                }

                int notification = duck.update_life();
                duck.update_position();
                duck.update_weapon();

                // std::cout << "Despues de mover las balas \n";

                // Si el pato murio, avisamos al cliente
                //estaria bueno mover esto 

                if(notification == 0){
                        //HELMET BROKE
                        Message broken_helmet_message;
                        duck.get_duck_broke_helmet_message(broken_helmet_message);
                        monitor.broadcast(broken_helmet_message);
                }
                if(notification == 1){
                        //ARMOR BROKE
                        Message broken_armor_message;
                        duck.get_duck_broke_armor_message(broken_armor_message);
                        monitor.broadcast(broken_armor_message);
                }

                Message kill_duck_message;
                if (duck.get_duck_dead_message(kill_duck_message)) {
                        monitor.broadcast(kill_duck_message);
                        std::cout << "Pato muerto."  << std::endl;
                }
        }
}
/* 
void Game::add_projectile(std::unique_ptr<Proyectil> projectile) {
    // Transfiere la propiedad del proyectil usando std::move
    projectiles.push_back(std::move(projectile));

} */

void Game::set_players(int number_of_players){
        players = number_of_players;
}



void Game::run() {

        map.setEscenario();
        create_ducks(players);

        send_map_message();

        //MANDO LOS MENSAJES CON LA POSICION DE LOS SPAWN PLACES
        create_spawn_places();
        send_spawn_place_message();

        uint64_t counter_bullets = 0;


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

                // envio las actualizaciones a los jugadores
                send_updates();

                if (check_end_of_round()){

<<<<<<< HEAD
                        if (round_manager.check_end_of_match()){
                                //vaciar la queue del juego
                                //mandarle al cliente que termino el partido y quien gano
                                notify_players_end_game();
                                //mandarle mas info sobre los puntajes de la partida

                        break;

                        } else {
                                std::cout << "Envio mensajes para iniciar la nueva ronda"  << std::endl;

                                initialize_round();
                                //avisarle al cliente que empieza un nuevo round
                                notify_players_end_round();
                                //pasarle al cliente el nuevo mapa
                                //send_map_message();
                                //send_spawn_place_message();
                                
                                //vaciar la queue del juego para descartar cualquier comando viejo
=======
                        //quizas se pueda hacer un get_duck_bullet_position() en vez de esto
                        //o sea mover la creacion del mensaje dentro del pato
                        if (counter_bullets % 3 == 0) {
                                if(duck.weapon != nullptr){
                                        for (std::unique_ptr<Projectile>& unique_proyectile : duck.weapon->projectiles) {
                                                Projectile* projectile = unique_proyectile.get();
                                                Message projectile_message;
                                                if (projectile->get_projectile_message(projectile_message)){
                                                        monitor.broadcast(projectile_message);
                                                }
                                        }
                                }
>>>>>>> origin
                        }
                }
                counter_bullets ++;

                //esta verificacion luego se va a ir
                if (check_end_game()){
                        notify_players_end_game();
                        is_running = false;
                        is_over = true;
                        break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(60));

                //map.tellMap();

        }
        std::cout << "Termino el juego!"  << std::endl;

}

void Game::send_updates(){
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
                                if (bullet.get_bullet_message(bullet_message)){
                                        monitor.broadcast(bullet_message);
                                }
                        }
                }
        }

}

void Game::notify_players_end_game(){
        Message msg;
        msg.type = END_GAME;
        msg.duck_winner = round_manager.get_duck_winner();
        monitor.broadcast(msg);
        std::cout << "Le aviso a los jugadores que el juego termino"  << std::endl;
}

void Game::notify_players_end_round(){
        Message msg;
        msg.type = END_ROUND;
        msg.duck_winner = round_manager.get_duck_round_winner();
        monitor.broadcast(msg);
        std::cout << "Le aviso a los jugadores que el juego termino"  << std::endl;
}

bool Game::check_end_of_round(){
        int ducks_alive = 0;
        for (Duck& duck : ducks) {
                if (!duck.is_dead) {
                        ducks_alive += 1;
                }
        }

        if(ducks_alive == 1){
                for (Duck& duck : ducks) {
                        if (!duck.is_dead) {
                                round_manager.declare_round_winner(duck.get_id());
                                std::cout << "Termino una ronda!"  << std::endl;
                                std::cout << "El ganador fue el pato "<< duck.get_id() << std::endl;
                        }
                }
        }

        return (ducks_alive == 1);

}

bool Game::check_end_game(){
        //checkear las condiciones necesarias para que termine un juego
        bool end = true;
        for (Duck& duck : ducks) {
                if (!duck.is_dead) {
                        end = false;
                }
        }

        return end;

}

void Game::stop() {
        std::cout << "Comienzo el stop..."  << std::endl;
        game_queue.close();
        monitor.remove_all_queues();
        is_running = false;
        is_over = true;
        std::cout << "Termino el stop"  << std::endl;
}

<<<<<<< HEAD
void Game::initialize_round() {
        //TODO: MAPA Y LOS OBJETOS ESTAN TODOS HARDCODEADO Y ES SIEMPRE IGUAL
        //cuando cambia la ronda deberia aparecer un mapa nuevo al azar
        map.setEscenario();
        initialize_ducks(); //reseteo los patos para que revivan y pierdan sus armas
        create_spawn_places();

<<<<<<< HEAD
=======
        Sniper* weapon = new Sniper();

        duck.setWeapon(weapon);
        Helmet* helmet = new Helmet(5,5); //le pongo posicion pero no importa porque se la asigno al pato

        duck.setHelmet(helmet);
    }
>>>>>>> origin
=======
void Game::inicializate_map() {
    // Le doy armas a los patos para probar

>>>>>>> origin
}

void Game::initialize_ducks(){
        for (Duck& duck : ducks) {
                Position pos = get_random_position_for_duck(duck.get_id());
                duck.reset_for_round(pos);
        }
}

//TODO: Esto solo sirve para la  distribucion de obstaculos hardcodeados
Position Game::get_random_position_for_duck(char duck_id){
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distrib_x(18, map.get_width() - 18);
        std::uniform_int_distribution<> distrib_y(10, map.get_height() - 20);


        int random_x = distrib_x(gen);
        int random_y = distrib_y(gen);

        bool has_place = map.placeDuck(random_x, random_y, duck_id);
        while (!has_place) {
                random_x = distrib_x(gen);
                random_y = distrib_y(gen);

                has_place = map.placeDuck(random_x, random_y, duck_id);
        }

        return Position(random_x, random_y);
        
}



void Game::create_ducks(int size) {
        round_manager.initialize_manager(size);

        for(uint16_t id= 1; id <= size; ++id) {
                char char_id = static_cast<char>(id + '0');
                Position pos = get_random_position_for_duck(char_id);

                ducks.emplace_back(char_id, pos.x, pos.y, &map);
        }
}

void Game::create_spawn_places() {
    //CREO ITEMS PARA METER EN LOS SPAWN PLACES
    std::cout << "CREO LOS ITEMS" << "\n";


    std::unique_ptr<Item> item1 = std::make_unique<Shotgun>(30, 130);
    std::unique_ptr<Item> item2 = std::make_unique<Armor>(100, 130);
    std::unique_ptr<Item> item3 = std::make_unique<PewPewLaser>(30, 75);
    std::unique_ptr<Item> item4 = std::make_unique<Helmet>(100, 75);




    //seteo N spawn places (4)
    std::cout << "CREO LOS SPAWN PLACES" << "\n";

    spawn_places.emplace_back(std::make_unique<SpawnPlace>(Position(30, 130), 0, item1->getItemId()));  
    spawn_places.emplace_back(std::make_unique<SpawnPlace>(Position(100, 130), 1, item2->getItemId()));  
    spawn_places.emplace_back(std::make_unique<SpawnPlace>(Position(30, 75), 2, item3->getItemId()));  
    spawn_places.emplace_back(std::make_unique<SpawnPlace>(Position(100, 75), 3, item4->getItemId()));


    //guardo los items en el vector de items despues de acceder a su item id, porque sino ya no tengo la refe
    items.push_back(std::move(item1));
    items.push_back(std::move(item2));
    items.push_back(std::move(item3));
    items.push_back(std::move(item4));


    
}

void Game::send_spawn_place_message(){
        for (int i = 0; i < N_SPAWN_PLACES; i++){
                Message spawn_place_position_message;
                spawn_places[i]->getSpawnPlacePositionMessage(spawn_place_position_message);
                monitor.broadcast(spawn_place_position_message);
        }
}

void Game::send_map_message(){
        Message message;
        message.type = MAP_INICIALIZATION;
        message.map = map.getMap();

        monitor.broadcast(message);
}

Duck* Game::getDuckById(char id) {
        for (auto& duck : ducks) {
                if (duck.get_id() == id) {
                        return &duck; // Retorna un puntero al pato encontrado
                }
        }
        return nullptr; // Retorna nullptr si no se encuentra el pato
}

//REFACTOR! ESTOY BUSCANDO EL ITEM Y EL SPAWN PLACE

std::shared_ptr<Item> Game::getItemByPosition(Position position) {
    int area_x_min = position.x;
    int area_x_max = position.x + DUCK_SIZE_X;
    int area_y_min = position.y;
    int area_y_max = position.y + DUCK_SIZE_Y;

    std::cout << "Buscando item en el área del pato desde X: " << area_x_min << " hasta X: " << area_x_max
              << ", y desde Y: " << area_y_min << " hasta Y: " << area_y_max << "\n";

    for (auto& item_ptr : items) {
        Position itemPos = item_ptr->getPosition();

        std::cout << "Item actual, de Id " << item_ptr->getItemId() << ", y posicion x: " << itemPos.x << " y: " << itemPos.y << std::endl;


        if (itemPos.x >= area_x_min && itemPos.x < area_x_max &&
            itemPos.y >= area_y_min && itemPos.y < area_y_max) {
            std::cout << "Encontré un item, en la pos x: " << itemPos.x << " y: " << itemPos.y << std::endl;
            //debo eliminarlo de la lista
            return item_ptr; 
        }
    }

    return nullptr; 
}

SpawnPlace* Game::getSpawnPlaceByPosition(Position position) {
    // Coordenadas del área del pato
    int area_x_min = position.x;
    int area_x_max = position.x + DUCK_SIZE_X;
    int area_y_min = position.y;
    int area_y_max = position.y + DUCK_SIZE_Y;

    std::cout << "Buscando SpawnPlace en el área del pato desde X: " << area_x_min 
              << " hasta X: " << area_x_max
              << ", y desde Y: " << area_y_min 
              << " hasta Y: " << area_y_max << "\n";

    for (auto& spawn_place_ptr : spawn_places) {
        Position spawnPlacePos = spawn_place_ptr->getPosition();

        if (spawnPlacePos.x >= area_x_min && spawnPlacePos.x < area_x_max &&
            spawnPlacePos.y >= area_y_min && spawnPlacePos.y < area_y_max) {
            std::cout << "Encontré un spawn place " << std::endl;
            return spawn_place_ptr.get();
        }
    }

    return nullptr; 
}


void Game::game_broadcast(Message message){
        monitor.broadcast(message);
}
