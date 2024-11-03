#include "game.h"
#include "common/constants.h"

//TODO: Tamanio del mapa hardcodeado
Game::Game(uint16_t match_id, GameQueueMonitor& monitor):
match_id(match_id), monitor(monitor), is_running(true), game_queue(), map(15, 10){}

Queue<std::shared_ptr<Executable>>& Game::get_game_queue(){
        return game_queue;
}


void Game::run() {

        //Creo el mapa con los objetos fijos (bloques) y la posicion inicial de los patos
        inicializate_map();
        map.printMap();


        //Mando la posicion de todo el mapa por primera vez para tener referencia de donde estan
        //Todos los obstaculos
        Message message;
        message.type = MAP_INICIALIZATION;
        message.player_id = 0; //tengo que poner esto POR EL ORDEN DE LO QUE MANDAMOS, HAY QUE CAMBIAR PARA MANDAR SIEMPRE PRIMERO EL TYPE ASI NO HACE FALTA
        message.map = map.getMap();

        monitor.broadcast(message);


        while (is_running) {
                // saco de 5 comandos de la queue y los ejecuto
                int i = 0;
                std::shared_ptr<Executable> command;
                while( i < 5 && game_queue.try_pop(command)){
                        
                        command->execute(*this);
                        i += 1;
                }

                

                //monitor.broadcast();
                // broadcast(); para avisarle a los jugadores lo que cambió

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

}

void Game::inicializate_map(){

        for (const Duck& duck : ducks) {
        int x = duck.get_x(); 
        int y = duck.get_y();
        
        if (!map.placeDuck(x, y)) {
            std::cout << "No se pudo colocar el pato en (" << x << ", " << y << ")\n";
        } else {
            std::cout << "Pato colocado en (" << x << ", " << y << ")\n";
        }
    }

}

//TODO: Esto solo sirve para dos patos y siempre tiene en cuenta que es el mismo distribucion de obstaculos

void Game::create_ducks(const std::vector<uint16_t>& ids) {
    // Lista de posiciones fijas donde se colocarán los patos
    std::vector<std::pair<int, int>> positions = {{1, 1}, {6, 1}};
    
    for (size_t i = 0; i < ids.size() && i < positions.size(); ++i) {
        // Obtén la posición fija para el pato actual
        int fixed_x = positions[i].first;
        int fixed_y = positions[i].second;
        
        ducks.emplace_back(ids[i], fixed_x, fixed_y, map); 
    }
}


Duck* Game::getDuckById(uint16_t id) {
    for (auto& duck : ducks) {
        if (duck.get_id() == id) {
            return &duck; // Retorna un puntero al pato encontrado
        }
    }
    return nullptr; // Retorna nullptr si no se encuentra el pato
}



/*

void Game::initialize_map(Update update) {
        
                Things to initialize:
                - Players
                - Boxes
                - Spawn places
                - Stage grounds
                - Map limit (if we model it)
        
}

void Game::check_move_effects(Duck moving_duck){
        // Check if after this movement, duck is hit by a projectile

        // Maybe there should be a simulation round in which where make all this things instead of this
}

bool Game::check_valid_position(int duck_x_position, int duck_y_position) {
        // Basically check if does not collisionates with a structure or a box
        return true;
}


void Game::duck_move(int player_id) {
        Duck selected_duck = get_duck();

        if (selected_duck.direction == 0){
                // Move left
                selected_duck.x_position--;
                if (!check_valid_position(selected_duck.x_position, selected_duck.y_position))
                        selected_duck.x_position++;
        } else {
                // Move right
                selected_duck.x_position++;
                if (!check_valid_position(selected_duck.x_position, selected_duck.y_position))
                        selected_duck.x_position--;
        }
        
        check_move_effects();
}

bool Game::is_touching_floor(int duck_x_position, int duck_y_position) {
        // Check if duck is touching floor with its feet
        return true;
}

void Game::duck_jump(int player_id) {
        Duck selected_duck = get_duck();

        if (is_touching_floor(selected_duck.x_position, selected_duck.y_position)){
                // Jump
                duck.y_position++;
                if (!check_valid_position(duck.x_position, duck.y_position))
                        duck.y_position--;
        } else {
                // Flutter

                // Here idk if use a fluttering bool attribute (and decrease falling speed) or just increment position in 1

        }
        
        check_move_effects();
}

void Game::duck_shoot(int player_id) {
        Duck selected_duck = get_duck();

        // Make a big if-else chain depending on the gun equipped
}

void Game::duck_take_item(int player_id) {
        Duck selected_duck = get_duck();

        // Remember that ammo may be different between guns
}

void Game::duck_drop_gun(int player_id) {
        Duck selected_duck = get_duck();

        // Drop ammo?
}

void Game::duck_drop_armor(int player_id) {
        Duck selected_duck = get_duck();

        
}

void Game::duck_aim_up(int player_id) {
        Duck selected_duck = get_duck();

}

void Game::duck_floor(int player_id) {
        Duck selected_duck = get_duck();

        // what happens when a duck floors in the air?

}

void Game::process_update(Update update) {
        
                Possible duck updates:
                - Move (left or right)
                - Jump or Flutter (slow the fall). Since it depends on if the duck is in ground or not it's better the server determine it
                - Shoot (to looking direction)
                - Take/leave a gun
                - Take/leave a helmet/armor
                - Aim up
                - Floor (stay down)

                Could make a function that simulates a movement, and returns a boolean 
                if it has moved (to only broadcast changes)

                If a change is done on the state we can put it on a list to broadcast

                Remember that in this function when a duck is hit by an attack we update stats
                The constants are going to be configurable by a yaml, but that could be a later refactor
        

        if (update.type == update_type::Move) {
                move_duck_move(update.player_id);
        } else if (update.type == update_type::Jump) {
                duck_jump(update.player_id);
        } else if (update.type == update_type::Shoot) {
                duck_shoot(update.player_id);
        } else if (update.type == update_type::TakeGunOrArmor) {
                duck_take_item(update.player_id);
        } else if (update.type == update_type::LeaveGun) {
                duck_drop_gun(update.player_id);
        } else if (update.type == update_type::LeaveArmor) {
                duck_drop_armor(update.player_id);
        } else if (update.type == update_type::AimUp) {
                duck_aim_up(update.player_id);
        } else if (update.type == update_type::Floor) {
                duck_floor(update.player_id);
        } else {
                std::cout << "Command not supported" << std::endl
        }
        
}

void Game::broadcast_changes(Update update) {
        // Broadcast the state changes of the list
        // Maybe we could translate all the changes, with a protocol that specifies the changes ammount

        // TODO: implement monitor
        // monitor.broadcast(message);

}

void Game::renew_iteration(Update update) {
        // Reset variables, calculate the sleep time and sleep
}

void Game::run() {
        while (is_running) {
                int current_round = game_round;
                // Should make more things with the round setUp, but the round feature was not implemented yet

                while (current_round == game_round && is_running) {
                        std::vector<Update> updates;
                        bool read_successful = true;

                        // Receive updates
                        while (read_successful) {
                                Update read_update;
                                read_successful = InputQueue::read_command(read_update); // To do: impl InputQueue
                                if (read_successful) {
                                        updates.push_back(read_update);
                                }
                        }

                        // Process updates
                        for (Update& update: updates) {
                                process_update(update);
                        }

                        // Simulate one round???

                        // Broadcast clients
                        broadcast_changes();

                        // Check end of the round conditions


                        renew_iteration();

                }

                current_round++;
        }
}


*/

void Game::stop() { is_running = false; }