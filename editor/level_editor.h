#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "common/constants.h"

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor();

    bool init();
    void run();
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
};
