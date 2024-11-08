#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2/SDL.h>

#include "common/command.h"
#include "common/queue.h"
#include "common/constants.h"
#include "common/message.h"

#include "client_game_map.h" // Si esta es la ubicación del archivo que declara ClientGameMap

struct Duck {
    int x, y;
    SDL_RendererFlip flipType;
    bool isMoving;
    int animationFrame; // Contador para controlar la animación
    int currentFrameIndex; // Índice del fotograma actual en el *sprite sheet*
    int frameWidth;
    int frameHeight;
};

struct Crate {
    int x, y;
};

struct GameState {
    Duck ducks[MAX_PLAYERS];
    int ducks_quantity;
    std::vector<Crate> crates;
    SDL_Texture *crate, *duck_t, *background;
    SDL_Renderer* renderer;
    //std::vector<std::vector<char>> *map;
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
    SDL_Surface* loadImage(const std::string& name_img);
    void loadGame(GameState* game);
    int processEvents(SDL_Window* window, GameState* game, uint16_t id);
    void doRender(SDL_Renderer* renderer, GameState* game);
};

#endif //SDL_HANDLER_H
