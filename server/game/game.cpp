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

        for (Duck& duck : ducks) {
                if (duck.is_dead) {
                        // Si el pato murio en la ronda anterior, lo saltamos y continuamos con el siguiente
                        continue;
                }

                int notification = duck.update_life();
                duck.update_position();
                duck.update_weapon();

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

void Game::add_projectile(std::unique_ptr<Proyectil> projectile) {
    // Transfiere la propiedad del proyectil usando std::move
    projectiles.push_back(std::move(projectile));

}

void Game::set_players(int number_of_players){
        players = number_of_players;
}



void Game::run() {

        map.setEscenario();
        create_ducks(players);

        Message message;
        message.type = MAP_INICIALIZATION;
        message.map = map.getMap();

        monitor.broadcast(message);

        //MANDO LOS MENSAJES CON LA POSICION DE LOS SPAWN PLACES
        create_spawn_places();


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

                if (check_end_game()){
                        notify_players_end_game();
                        is_running = false;
                        is_over = true;
                        break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(60));

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
        monitor.broadcast(msg);
        std::cout << "Le aviso a los jugadores que el juego termino"  << std::endl;
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
        std::cout << "Comienzo el stop"  << std::endl;
        game_queue.close();
        monitor.remove_all_queues();
        items.clear();
        ducks.clear();
        is_running = false;
        is_over = true;
        std::cout << "termino el stop"  << std::endl;
}

void Game::initialize_round() {
        //por ahora el escenario es unico y esta harcodeado
        //cuando cambia la ronda deberia aparecer un mapa nuevo al azar
        map.setEscenario();
        initialize_ducks();

}

void Game::initialize_ducks(){
        for (Duck& duck : ducks) {
                Position pos = get_random_position_for_duck(duck.get_id());
                duck.reset_for_round(pos);
        }
}

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



//TODO: Esto solo sirve para la  distribucion de obstaculos hardcodeados
void Game::create_ducks(int size) {
        for(uint16_t id= 1; id <= size; ++id) {
                char char_id = static_cast<char>(id + '0');
                Position pos = get_random_position_for_duck(char_id);
                Duck(char_id, pos.x, pos.y, &map);
                ducks.emplace_back(char_id, pos.x, pos.y, &map);
        }
}

//REFACTOR!!!!
//Mi duda existencial es si spawn place vale la pena como clase o simplemente deberias ser
//Unas posiciones x e y constantes donde hago aparecer a los items
void Game::create_spawn_places() {
    //CREO ITEMS PARA METER EN LOS SPAWN PLACES
    std::cout << "CREO LOS ITEMS" << "\n";


    std::unique_ptr<Item> item1 = std::make_unique<Weapon>("Nombre", 100.0, 1.5, 30, 30, 130);
    std::unique_ptr<Item> item2 = std::make_unique<Armor>(100, 130);
    std::unique_ptr<Item> item3 = std::make_unique<Weapon>("Nombre", 100.0, 1.5, 30, 30, 75);
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


    for (int i = 0; i < N_SPAWN_PLACES; i++){
        Message spawn_place_position_message;
        spawn_places[i]->getSpawnPlacePositionMessage(spawn_place_position_message);
        monitor.broadcast(spawn_place_position_message);
    }
}

void Game::create_items() {

    std::srand(static_cast<unsigned>(std::time(nullptr))); // Inicializar la semilla aleatoria

    for (int i = 0; i < 1; ++i) {
        int x = 30;//std::rand() % map.get_width();  // Generar posición aleatoria en el mapa
        int y = 125;//std::rand() % map.get_height();

        // Crear un tipo de ítem aleatorio
        int item_type = 1;//std::rand() % 3;
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

//REFACTOR! ESTOY BUSCANDO EL ITEM Y EL SPAWN PLACE
Item* Game::getItemByPosition(Position position) {
    // Coordenadas del área del pato
    int area_x_min = position.x;
    int area_x_max = position.x + DUCK_SIZE_X;
    int area_y_min = position.y;
    int area_y_max = position.y + DUCK_SIZE_Y;

    std::cout << "Buscando item en el área del pato desde X: " << area_x_min << " hasta X: " << area_x_max
              << ", y desde Y: " << area_y_min << " hasta Y: " << area_y_max << "\n";

    for (auto& item_ptr : items) {
        Position itemPos = item_ptr->getPosition();


        if (itemPos.x >= area_x_min && itemPos.x < area_x_max &&
            itemPos.y >= area_y_min && itemPos.y < area_y_max) {
            return item_ptr.get(); 
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
            return spawn_place_ptr.get();
        }
    }

    return nullptr; 
}


void Game::game_broadcast(Message message){
        monitor.broadcast(message);
}
