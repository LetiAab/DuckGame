#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

#include "common/thread.h"

struct Duck {
    int x, y;
    SDL_RendererFlip flipType;
};

struct Crate {
    int x, y;
};

struct GameState {
    Duck duck;
    Crate crates[4];
    SDL_Texture *crate, *duck_t, *background;
    SDL_Renderer* renderer;
};

class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    void run();

private:
    SDL_Surface* loadImage(const std::string& name_img);
    void loadGame(GameState* game);
    int processEvents(SDL_Window* window, GameState* game);
    void doRender(SDL_Renderer* renderer, GameState* game);
};

#endif //SDL_HANDLER_H
