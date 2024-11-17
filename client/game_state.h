#ifndef GAME_STATE_H
#define GAME_STATE_H
#include <SDL_render.h>

#include "client_game_map.h" // Si esta es la ubicación del archivo que declara ClientGameMap
#include "common/command.h"
#include "common/constants.h"
#include "common/queue.h"

struct Duck {
    int x, y;
    SDL_RendererFlip flipType;
    bool is_moving;
    bool is_jumping;
    bool is_fluttering;

    int animation_frame; // Contador para controlar la animación
    int current_frame_index; // Índice del fotograma actual en el *sprite sheet*
    int frame_width;
    int frame_height;


};

struct Bullet {
    int x, y;
    int animation_frame; // Contador para controlar la animación
    int current_frame_index; // Índice del fotograma actual en el *sprite sheet*
    int frame_width;
    int frame_height;
};

struct Crate {
    int x, y;
};

struct GameState {
    Duck ducks[MAX_PLAYERS];
    int ducks_quantity;
    std::vector<Crate> crates;
    SDL_Renderer* renderer;
    ClientGameMap client_game_map;
    Queue<Command>* command_queue;

    // Constructor para inicializar client_game_map
    GameState() : client_game_map(MATRIX_M, MATRIX_N) {}

};

#endif //GAME_STATE_H