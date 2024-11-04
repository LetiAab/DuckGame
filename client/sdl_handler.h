#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2/SDL.h>

#include "common/command.h"
#include "common/queue.h"
#include "common/constants.h"

struct Duck {
    int x, y;
    SDL_RendererFlip flipType;
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
    std::vector<std::vector<int>> *map;
    Queue<Command>* command_queue;
};

class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    void run(std::vector<std::vector<int>> &map, Queue<Command>& command_queue, uint16_t id);

private:
    SDL_Surface* loadImage(const std::string& name_img);
    void loadGame(GameState* game);
    int processEvents(SDL_Window* window, GameState* game, uint16_t id);
    void doRender(SDL_Renderer* renderer, GameState* game);
};

#endif //SDL_HANDLER_H
