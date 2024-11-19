#ifndef SDL_RENDERERMANAGER_H
#define SDL_RENDERERMANAGER_H
#include <SDL_render.h>

#include "game_state.h"
#include "sdl_texturehandler.h"
#include "common/message.h"

class RendererManager {
public:
    RendererManager(SDL_Renderer* renderer, TextureHandler& texture_handler);
    void doRenderStatic(GameState* game);
    void doRenderDynamic(GameState* game, Message& message);

private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
    void renderBullet(GameState* game,  int size = 20);
    void renderDucks(GameState* game);
    void renderItems(GameState* game);


};

#endif //SDL_RENDERERMANAGER_H
