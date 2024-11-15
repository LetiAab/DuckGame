#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include "game_initializer.h"
#include "common/message.h"
#include "sdl_texturehandler.h"
#include "game_state.h"
#include "sdl_eventprocessor.h"
#include "sdl_renderermanager.h"

class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    void run(std::vector<std::vector<char>> &map, Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue);

private:
    TextureHandler handle_textures;
    EventProcessor eventProcessor;
    GameInitializer gameInitializer;
    std::unique_ptr<RendererManager> rendererManager;
    void loadGame(GameState* game);
    void showStartScreen(SDL_Renderer* renderer);
    int waitForStartGame(SDL_Renderer* renderer);
    void showLobbyScreen(SDL_Renderer* renderer);
};

#endif //SDL_HANDLER_H
