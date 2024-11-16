#include "game_initializer.h"

#include <iostream>

void GameInitializer::initializeDucks(GameState* game, const int frame_width, const int frame_height) {
    Duck duck{};
    int count = 0;
    game->ducks_quantity = 0;
    for (size_t i = 0; i < game->client_game_map.map.size(); ++i) {
        for (size_t j = 0; j < (game->client_game_map.map)[i].size(); ++j) {
            // Verificar si el valor está entre '1' y '6'
            if ((game->client_game_map.map)[i][j] >= '1' && (game->client_game_map.map)[i][j] <= '6') {
                duck.x = j * TILE_SIZE;
                duck.y = i * TILE_SIZE;
                duck.flipType = SDL_FLIP_NONE;
                duck.is_moving = false;
                duck.animation_frame = 0;
                duck.current_frame_index = 0;
                
                //Items
                duck.armor_equiped = 0;
                duck.weapon_equiped = 0;
                duck.helmet_equiped = 0;
                duck.item_on_hand = 0;

                // Configuro el tamaño de los fotogramas del pato
                duck.frame_width = frame_width;
                duck.frame_height = frame_height;

                count++;
                if (count == DUCK_TOTAL_SIZE) {
                    game->ducks[game->ducks_quantity] = duck;
                    game->ducks_quantity++;
                    count = 0;
                }
            }
        }
    }
    std::cout << "Cantidad de patos: " << game->ducks_quantity << "\n";
}

void GameInitializer::initializeCrates(GameState* game) {
    for (size_t i = 0; i < game->client_game_map.map.size(); ++i) {
        for (size_t j = 0; j < (game->client_game_map.map)[i].size(); ++j) {
            if ((game->client_game_map.map)[i][j] == 'P') {
                Crate crate;
                crate.x = j * TILE_SIZE;
                crate.y = i * TILE_SIZE;
                game->crates.push_back(crate);
            }
        }
    }
}

