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

    //EL pato tiene los ids de las armas/armaduras que tiene. Si es 0 no tiene ninguna
    uint8_t weapon_equiped;
    uint8_t helmet_equiped;
    uint8_t armor_equiped;

    uint8_t item_on_hand;



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
    int type;
};

// This does not contemplate bananas or grenades, that can keep in ground
// Implementate them implies using a protocol with an eliminate_bullet option
// Or using an 'updated' flag
struct Projectile {
    int current_x;
    int current_y;
    int old_x;
    int old_y;
    int id;
    int type;
    int times_repeated;
};

struct Crate {
    int x, y;
};

struct Box {
    int x, y;
    uint8_t item_id;
    bool destroyed;
    bool item_taked;
};

struct SpawnPlace {
    int x, y;
    uint8_t item_id;
};

struct GameState {
    //Duck ducks[MAX_PLAYERS];
    std::vector<Duck> ducks;
    int ducks_quantity;
    std::vector<Crate> crates;
    std::vector<SpawnPlace> spawn_places;
    std::vector<Box> boxes;
    SDL_Renderer* renderer;
    ClientGameMap client_game_map;
    Queue<Command>* command_queue;
    std::vector<Projectile> projectiles;


    // Constructor para inicializar client_game_map
    GameState() : client_game_map(MATRIX_M, MATRIX_N) {}

};

#endif //GAME_STATE_H