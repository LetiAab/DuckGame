#include "game_initializer.h"

#include <iostream>


void GameInitializer::initialize_new_round(GameState &game, Queue<Message> &message_queue){
    //Respetar este orden por que es el orden en que el server envia los mensajes
    initializeMap(game, message_queue);

    initializeDucks(game, message_queue);

    initializeSpawnPlaces(game, message_queue);
    
}


void GameInitializer::initializeGame(Queue<Message> &message_queue, GameState& game, int frame_width, int frame_height){
    //Respetar este orden por que es el orden en que el server envia los mensajes
    initializeMap(game, message_queue);

    createDucks(message_queue, game, frame_width, frame_height);

    initializeSpawnPlaces(game, message_queue);

    initializeBoxes(game, message_queue);

}


void GameInitializer::initializeMap(GameState& game, Queue<Message> &message_queue){
    Message message_map = message_queue.pop();

    if(message_map.type == MAP_INICIALIZATION){
        std::cout << "INICIALIZO EL MAPA "<< "\n";
        game.client_game_map.setMap(message_map.map);
    }

    initializeCrates(game);
}

void GameInitializer::initializeSpawnPlaces(GameState& game, Queue<Message> &message_queue){
    game.spawn_places.clear(); // borro los anteriores si habia

    std::cout << "INICIALIZO LOS SPAWNS" << "\n";
    for (int i = 0; i < N_SPAWN_PLACES; i++){

        Message message = message_queue.pop();

        if(message.type == SPAWN_PLACE_POSITION){
            game.spawn_places.emplace_back(message.spaw_place_x * TILE_SIZE, message.spaw_place_y * TILE_SIZE, message.item_id);
        }
    }
}

void GameInitializer::initializeBoxes(GameState& game, Queue<Message> &message_queue){
    game.boxes.clear(); // borro los anteriores si habia

    for (int i = 0; i < N_BOXES; i++){
        Message message = message_queue.pop();

        if(message.type == BOX_POSITION){
            game.boxes.emplace_back(message.box_x * TILE_SIZE, message.box_y * TILE_SIZE, message.item_id);
        }
    }
}


void GameInitializer::initializeDucks(GameState &game, Queue<Message> &message_queue){
    Message message_ducks = message_queue.pop();
    if(message_ducks.type == DUCKS_INICIALIZATION){

        game.ducks_quantity = message_ducks.ducks_quantity;

        std::cout << "Recibo los patos de nuevo" << "\n";
        for (int i = 0; i < game.ducks_quantity; i++){

            Message msg = message_queue.pop();

            if (msg.type == DUCK_POS_UPDATE){
                        int pos_id = msg.player_id - 1;

                        game.ducks[pos_id].x = msg.duck_x * TILE_SIZE;
                        game.ducks[pos_id].y = msg.duck_y * TILE_SIZE;

                        if(msg.looking == LOOKING_LEFT){
                            //pato esta mirando a la izquierda
                            game.ducks[pos_id].flipType = SDL_FLIP_HORIZONTAL;

                        } else {
                            //pato esta mirando a la derecha
                            game.ducks[pos_id].flipType = SDL_FLIP_NONE;
                        }

                        game.ducks[pos_id].is_moving = msg.is_moving;
                        game.ducks[pos_id].is_jumping = msg.is_jumping;
                        game.ducks[pos_id].is_fluttering = msg.is_fluttering;

                        game.ducks[pos_id].armor_equiped = 0;
                        game.ducks[pos_id].weapon_equiped = 0;
                        game.ducks[pos_id].helmet_equiped = 0;
                        game.ducks[pos_id].item_on_hand = 0;

                        game.ducks[pos_id].current_frame_index = 0;
                        game.ducks[pos_id].animation_frame = 0;

            }
        }
    }

}

void GameInitializer::createDucks(Queue<Message> &message_queue, GameState &game, const int frame_width, const int frame_height) {
    Message message_ducks = message_queue.pop();
    if(message_ducks.type == DUCKS_INICIALIZATION){

        game.ducks_quantity = message_ducks.ducks_quantity;
        Duck duck{};

        std::cout << "INICIALIZO LOS PATOS" << "\n";
        for (int i = 0; i < game.ducks_quantity; i++){

            Message msg = message_queue.pop();

            if (msg.type == DUCK_POS_UPDATE){
                duck = Duck{};
                duck.x = msg.duck_x * TILE_SIZE;
                duck.y = msg.duck_y * TILE_SIZE;
                duck.flipType = SDL_FLIP_NONE;
                duck.is_moving = false;
                duck.is_jumping = false;
                duck.is_fluttering = false;

                duck.armor_equiped = 0;
                duck.weapon_equiped = 0;
                duck.helmet_equiped = 0;
                duck.item_on_hand = 0;

                duck.animation_frame = 0;
                duck.current_frame_index = 0;
                duck.frame_width = frame_width;
                duck.frame_height = frame_height;

                game.ducks.push_back(duck);
            }
        }
    }

}


void GameInitializer::initializeCrates(GameState& game) {
    game.crates.clear();
    for (size_t i = 0; i < game.client_game_map.map.size(); ++i) {
        for (size_t j = 0; j < (game.client_game_map.map)[i].size(); ++j) {
            if ((game.client_game_map.map)[i][j] == 'P') {
                Crate crate;
                crate.x = j * TILE_SIZE;
                crate.y = i * TILE_SIZE;
                game.crates.push_back(crate);
            }
        }
    }
}



