#ifndef SDL_SCREENMANAGER_H
#define SDL_SCREENMANAGER_H
#include "sdl_texturehandler.h"

class ScreenManager {
public:
    ScreenManager(SDL_Renderer *renderer, TextureHandler &texture_handler);
    void showStartScreen();
    void showLobbyScreen();
private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
};

#endif //SDL_SCREENMANAGER_H
