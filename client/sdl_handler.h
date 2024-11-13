#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2/SDL.h>
#include <unordered_map>

#include "common/command.h"
#include "common/queue.h"
#include "common/constants.h"
#include "common/message.h"

#include "client_game_map.h" // Si esta es la ubicación del archivo que declara ClientGameMap
#include "sdl_handletextures.h"

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

class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    void run(std::vector<std::vector<char>> &map, Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue);

private:
    SDLHandleTextures handle_textures;
    std::unordered_map<SDL_Keycode, bool> keyState; //necesito esto para que se mande un solo comando
    void loadGame(GameState* game);
    void initializeDucks(GameState* game, int frame_width, int frame_height);
    void initializeCrates(GameState* game);
    int processEvents(SDL_Window* window, GameState* game, uint16_t id);
    void doRenderDynamic(SDL_Renderer* renderer, GameState* game, Message& message);
    void doRenderStatic(SDL_Renderer* renderer, GameState* game);
    void render_bullet(SDL_Renderer* renderer, int x, int y, int size);
};

#endif //SDL_HANDLER_H
