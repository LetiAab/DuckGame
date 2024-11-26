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
    void doRenderDynamic(GameState* game, Message& message, uint16_t id);

    void refreshStatic(GameState* game);

private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
    SDL_Texture* static_scene;

    void renderBullet(GameState* game,  int size = 20);
    void renderDucks(GameState* game);
    void renderItems(GameState* game);
    void renderStats(GameState* game, uint16_t id);

    void renderBoxes(GameState* game);

    void renderItem(uint8_t item_id, int x, int y, int mult = 1);    



};

#endif //SDL_RENDERERMANAGER_H
