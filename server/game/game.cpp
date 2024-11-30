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

void Game::add_throwed_weapon(Weapon* throwed_weapon) {
        if (throwed_weapon->getItemId() == GRENADE_ID) {
                auto shared_throwed_weapon = std::shared_ptr<Grenade>((Grenade*)throwed_weapon);
                throwed_weapons.push_back(shared_throwed_weapon);
        } else if (throwed_weapon->getItemId() == BANANA_ID) {
                auto shared_throwed_weapon = std::shared_ptr<Banana>((Banana*)throwed_weapon);
                throwed_weapons.push_back(shared_throwed_weapon);
        } else {
                std::cout << "Este arma no debería ser agregada! \n";
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

        for(auto it = throwed_weapons.begin(); it != throwed_weapons.end(); ) {
                Weapon* throwed_weapon = it->get();

                if (throwed_weapon->getItemId() == GRENADE_ID) {
                        Grenade* throwed_grenade = (Grenade*)throwed_weapon;
                        Position current_position = throwed_grenade->getPosition();

                        throwed_grenade->update_weapon(current_position.x, current_position.y, LOOKING_LEFT, &map, 0);
                } else if (throwed_weapon->getItemId() == BANANA_ID) {
                        Banana* throwed_banana = (Banana*)throwed_weapon;
                        // Position current_position = throwed_banana->getPosition();

                        throwed_banana->update_weapon(map);

/*                         if (throwed_banana->pisada) {
                              it = throwed_weapons.erase(it);
                        } */

                        std::cout << "Banana en pos x: " << throwed_banana->getPosition().x << " y: " << throwed_banana->getPosition().y << std::endl;
                }
                
                ++it;
        }
        
        for(const auto& box: boxes){
                if(box->isDestroyed()){
                        continue;
                }

                if(box->update_life(items)){
                        std::cout << "MANDO EL MENSAJE DE LA BALA" << "\n";
                        Message msg;
                        box->getBoxMessage(msg);
                        monitor.broadcast(msg);
                }

        }

        for (Duck& duck : ducks) {
                if (duck.is_dead) {
                        // Si el pato murio en la ronda anterior, lo saltamos y continuamos con el siguiente
                        // Nota: dejo esto para el caso en que la granada le explota en la mano al pato
                        duck.update_weapon();
                        continue;
                }

                int notification = duck.update_life();
                duck.update_position();

                for (auto& throwed_weapon: throwed_weapons) {
                        if (throwed_weapon->getItemId() == BANANA_ID) {
                                Banana* throwed_banana = (Banana*) throwed_weapon.get();
                                if (!throwed_banana->pisada) {
                                        Position throwed_pos = throwed_banana->getPosition();
                                        if (map.bullet_hit_other_duck(map.at(throwed_pos), 'z')) {
                                                std::cout << "Pisada ahora es true \n";
                                                throwed_banana->pisada = true;
                                                throwed_banana->setUsed(true);
                                                Message throwed_message;
                                                if (throwed_weapon->get_throwed_position_message(throwed_message)){
                                                        monitor.broadcast(throwed_message);
                                                } 
                                        }
                                }

                        }
                }

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

        level_manager.choose_level();

        map.set_escenario_for_round(level_manager.getMap());
        create_ducks(players, level_manager.get_ducks_positions());
        create_spawn_places(level_manager.get_spawn_places());
        create_boxes(level_manager.get_boxes());
        create_items_on_floor(level_manager.get_items());

        //envio al cliente los mensajes
        send_map_message();
        send_initialize_ducks_message();
        send_spawn_place_message();
        send_boxes_initialize_message();
        send_items_on_floor_message();

        while (is_running) {
                const auto start = std::chrono::high_resolution_clock::now();


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

                        int status = round_manager.check_match_status();

                        switch (status){
                        case MATCH_NEXT_ROUND:

                                initialize_round();

                                std::cout << "Envio mensajes para iniciar la nueva ronda"  << std::endl;

                                notify_players_end_round();
                                send_map_message();
                                send_initialize_ducks_message();
                                send_spawn_place_message();
                                send_boxes_initialize_message();
                                send_items_on_floor_message();

                                
                                //vaciar la queue del juego para descartar cualquier comando viejo?)
                                break;
                        
                        case MATCH_5_ROUNDS:
                                //TODO: Mandar primero un resumen de como va el juego

                                initialize_round();

                                std::cout << "Envio mensajes para iniciar la nueva ronda"  << std::endl;

                                notify_players_end_round();
                                send_map_message();
                                send_initialize_ducks_message();
                                send_spawn_place_message();
                                send_boxes_initialize_message();
                                send_items_on_floor_message();

                                
                                //vaciar la queue del juego para descartar cualquier comando viejo?)
                                break;

                        case MATCH_HAS_WINNER:

                                notify_players_end_game();
                                //mandarle mas info sobre los puntajes de la partida

                                is_running = false;
                                is_over = true;

                                break;
                        }
                }

                const auto end = std::chrono::high_resolution_clock::now();
                auto loop_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                auto sleep_duration = GAME_TIME_SLEEP - loop_duration;

                if (sleep_duration > 0) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));
                }
        }


        std::cout << "Termino el juego!"  << std::endl;
}



void Game::send_updates(){



        for (Duck& duck : ducks) {

                Message duck_message;
                if(duck.get_duck_position_message(duck_message)){
                        monitor.broadcast(duck_message);
                }

                if(duck.weapon != nullptr){
                        for (std::unique_ptr<Projectile>& unique_proyectile : duck.weapon->projectiles) {
                                Projectile* projectile = unique_proyectile.get();
                                Message projectile_message;
                                if (projectile->get_projectile_message(projectile_message)){
                                        monitor.broadcast(projectile_message);
                                }
                        }

                }

                // actualizo la posición de las balas de la granada
                for (auto& throwed_weapon: throwed_weapons) {
                        
                        Message throwed_message;
                        
                        if (throwed_weapon->get_throwed_position_message(throwed_message)){
                                monitor.broadcast(throwed_message);
                        } 


                        for (auto& unique_proyectile : throwed_weapon->projectiles) {
                                Projectile* projectile = unique_proyectile.get();
                                Message projectile_message;
                                if (projectile->get_projectile_message(projectile_message)){
                                        monitor.broadcast(projectile_message);
                                }
                        }
                }
                
        }

}



void Game::notify_players_end_game(){
         std::cout << "El ganador de la partida fue el pato "<< static_cast<char>(round_manager.get_duck_winner()) << std::endl;
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
        std::cout << "Le aviso a los jugadores que la ronda termino"  << std::endl;
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
        std::cout << "Comienzo el stop"  << std::endl;
        game_queue.close();
        monitor.remove_all_queues();
        is_running = false;
        is_over = true;
        std::cout << "termino el stop"  << std::endl;
}

void Game::initialize_round() {
        level_manager.choose_level();

        map.set_escenario_for_round(level_manager.getMap());
        initialize_ducks(level_manager.get_ducks_positions());
        create_spawn_places(level_manager.get_spawn_places());
        create_boxes(level_manager.get_boxes());
        create_items_on_floor(level_manager.get_items());


}


void Game::send_initialize_ducks_message(){
        //podria mandar todo en un solo mensaje pero primero necesito saber si anda
        Message ducks_message;
        ducks_message.type = DUCKS_INICIALIZATION;
        ducks_message.ducks_quantity = ducks.size();

        monitor.broadcast(ducks_message);

        for (Duck& duck : ducks) {

                Message duck_message;
                if(duck.get_duck_initialize_message(duck_message)){
                        monitor.broadcast(duck_message);
                }
        }
}


void Game::initialize_ducks(std::vector<Position> ducks_positions){

        for (std::size_t i = 0; i < ducks.size(); ++i) {
                Position pos = ducks_positions[i];
                map.placeDuck(pos, ducks[i].get_id());
                ducks[i].reset_for_round(pos);
        }

}


void Game::create_ducks(int size, std::vector<Position> ducks_positions) {
        round_manager.initialize_manager(size);

        for(uint16_t id = 1; id <= size; ++id) {
                char char_id = static_cast<char>(id + '0');
                Position pos = ducks_positions[id-1];
                map.placeDuck(pos, char_id);
                ducks.emplace_back(char_id, pos.x, pos.y, &map);
        }
}



void Game::create_spawn_places(std::vector<Position> spawns_positions) {
    std::cout << "CREO LOS SPAWN PLACES" << "\n";

    spawn_places.clear();

    for (size_t i = 0; i < spawns_positions.size(); ++i) {
        Position pos = spawns_positions[i];
        
        std::unique_ptr<Item> item = std::make_unique<Shotgun>(pos.x, pos.y);
        
        spawn_places.emplace_back(std::make_unique<SpawnPlace>(Position(pos.x, pos.y), i, item->getItemId()));

        items.push_back(std::move(item));
    }
}

void Game::create_boxes(std::vector<Position> boxes_positions){
        std::cout << "CREO " << boxes_positions.size() << " BOXES" << "\n";
        boxes.clear();

        for (size_t i = 0; i < boxes_positions.size(); ++i) {
                Position pos = boxes_positions[i];
                
                boxes.emplace_back(std::make_unique<Box>(pos, i, &map));
        }
}


void Game::create_items_on_floor(std::vector<ItemConfig> items_on_floor_positions){
        std::cout << "CREO ITEMES ON FLOOR: " << items_on_floor_positions.size()<< "\n";
        items_on_floor.clear();

        for (size_t i = 0; i < items_on_floor_positions.size(); ++i) {
                ItemConfig itemConfig = items_on_floor_positions[i];
                
                int x = itemConfig.x;
                int y = itemConfig.y;
                uint8_t item_id = itemConfig.item_id;
                
                if (item_id == ARMOR_ID) {
                        items_on_floor.emplace_back(std::make_shared<Armor>(x, y));
                }
                else if (item_id == HELMET_ID) {
                        items_on_floor.emplace_back(std::make_shared<Helmet>(x, y));
                }
                else if(item_id == PEW_PEW_LASER_ID){
                        items_on_floor.emplace_back(std::make_shared<PewPewLaser>(x, y));
                }
                else if (item_id == LASER_RIFLE_ID) {
                        items_on_floor.emplace_back(std::make_shared<LaserRifle>(x, y));
                }
                else if(item_id == AK_47_ID){
                        items_on_floor.emplace_back(std::make_shared<Ak47>(x, y));
                }
                else if (item_id == DUEL_PISTOL_ID) {
                        items_on_floor.emplace_back(std::make_shared<DuelPistol>(x, y));
                }
                else if(item_id == COWBOY_PISTOL_ID){
                        items_on_floor.emplace_back(std::make_shared<CowboyPistol>(x, y));
                }
                else if (item_id == MAGNUM_ID) {
                        items_on_floor.emplace_back(std::make_shared<Magnum>(x, y));
                }
                else if(item_id == SHOTGUN_ID){
                        items_on_floor.emplace_back(std::make_shared<Shotgun>(x, y));
                }
                else if(item_id == SNIPER_ID){
                        items_on_floor.emplace_back(std::make_shared<Sniper>(x, y));
                }
                else if(item_id == BANANA_ID){
                        items_on_floor.emplace_back(std::make_shared<Banana>(x, y));
                }
                else if(item_id == GRENADE_ID){
                        items_on_floor.emplace_back(std::make_shared<Grenade>(x, y));
                }
        }
}



void Game::send_boxes_initialize_message(){
        Message msg;
        msg.type = BOXES_INICIALIZATION;
        msg.boxes_quantity = boxes.size();
        monitor.broadcast(msg);

    for (size_t i = 0; i < boxes.size(); ++i){
        Message box_position_message;
        boxes[i]->getBoxPositionMessage(box_position_message);
        monitor.broadcast(box_position_message);
    }
}

void Game::send_items_on_floor_message(){

        std::cout << "MANDO LOS ITEMS ON FLOOR: " << items_on_floor.size() << "\n";

        Message msg;
        msg.type = ITEMS_ON_FLOOR_INICIALIZATION;
        msg.items_on_floor_quantity = items_on_floor.size();
        monitor.broadcast(msg);

    for (size_t i = 0; i < items_on_floor.size(); ++i){
        std::cout << "MANDO el item position" << "\n";

        Message item_position_message;
        items_on_floor[i]->getItemPositionMessage(item_position_message);
        

        monitor.broadcast(item_position_message);
    }
}


void Game::send_spawn_place_message(){
        Message msg;
        msg.type = SPAWN_PLACES_INICIALIZATION;
        msg.spawn_places_quantity = spawn_places.size();
        monitor.broadcast(msg);

        for (size_t i = 0; i < spawn_places.size(); ++i){
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


    for (auto it = items.begin(); it != items.end(); ++it) {
        Position itemPos = (*it)->getPosition();


        if (itemPos.x >= area_x_min && itemPos.x < area_x_max &&
            itemPos.y >= area_y_min && itemPos.y < area_y_max) {
            std::cout << "Encontré un item, en la pos x: " << itemPos.x << " y: " << itemPos.y << std::endl;

            auto foundItem = *it;

            items.erase(it);

            return foundItem;
        }
    }

    return nullptr; // No se encontró ningún item
}

std::shared_ptr<Item> Game::getItemOnFloorByPosition(Position position) {
    int area_x_min = position.x;
    int area_x_max = position.x + DUCK_SIZE_X;
    int area_y_min = position.y;
    int area_y_max = position.y + DUCK_SIZE_Y;


    for (auto it = items_on_floor.begin(); it != items_on_floor.end(); ++it) {
        Position itemPos = (*it)->getPosition();


        if (itemPos.x >= area_x_min && itemPos.x < area_x_max &&
            itemPos.y >= area_y_min && itemPos.y < area_y_max) {
            std::cout << "Encontré un item EN EL PISO, en la pos x: " << itemPos.x << " y: " << itemPos.y << std::endl;

            auto foundItem = *it;

            items_on_floor.erase(it);

            return foundItem;
        }
    }

    return nullptr; // No se encontró ningún item
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


Box* Game::getBoxByPosition(Position position) {
    // Coordenadas del área del pato
    int area_x_min = position.x;
    int area_x_max = position.x + DUCK_SIZE_X;
    int area_y_min = position.y;
    int area_y_max = position.y + DUCK_SIZE_Y;

    std::cout << "Buscando Box en el área del pato desde X: " << area_x_min 
              << " hasta X: " << area_x_max
              << ", y desde Y: " << area_y_min 
              << " hasta Y: " << area_y_max << "\n";

    for (auto& box : boxes) {
        Position boxPos = box->getPosition();

        if (boxPos.x >= area_x_min && boxPos.x < area_x_max &&
            boxPos.y >= area_y_min && boxPos.y < area_y_max) {
            std::cout << "Encontré una box, o donde había una  " << std::endl;
            return box.get();
        }
    }

    return nullptr; 
}

void Game::game_broadcast(Message message){
        monitor.broadcast(message);
}
