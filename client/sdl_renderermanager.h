#ifndef SDL_RENDERERMANAGER_H
#define SDL_RENDERERMANAGER_H
#include <SDL_render.h>

#include "camera.h"
#include "game_state.h"
#include "sdl_texturehandler.h"
#include "../common/message.h"

class RendererManager {
public:
    RendererManager(SDL_Renderer* renderer, TextureHandler& texture_handler, Camera& camera);
    void doRenderStatic(GameState* game);
    void doRenderDynamic(GameState* game, Message& message, uint16_t id);


private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
    SDL_Texture* static_scene;
    Camera& camera;
    int image_index;
    const std::vector<std::string> backgrounds;
    const std::vector<std::string> platforms;
    void renderBullet(GameState* game,  int size = 20);
    void renderDucks(GameState* game);
    void renderItems(GameState* game);
    void renderThrowed(GameState* game);
    void renderStats(GameState* game, uint16_t id);
    void renderBoxes(GameState* game);
    void renderItemsOnFloor(GameState* game);
    void renderItem(uint8_t item_id, int x, int y, int mult = 1, bool enableOscillation = true);    
    void updateCamera(GameState* game, Camera& camera);
    SDL_Rect transformToCameraSpace(float obj_x, float obj_y, float obj_w, float obj_h);
};

#endif //SDL_RENDERERMANAGER_H
