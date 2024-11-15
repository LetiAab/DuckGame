#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2/SDL.h>
#include "game_initializer.h"
#include "common/message.h"
#include "sdl_texturehandler.h"
#include "game_state.h"
#include "sdl_eventprocessor.h"

class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    void run(std::vector<std::vector<char>> &map, Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue);

private:
    TextureHandler handle_textures;
    EventProcessor eventProcessor;
    GameInitializer gameInitializer;
    void loadGame(GameState* game);
    void doRenderDynamic(SDL_Renderer* renderer, GameState* game, Message& message);
    void doRenderStatic(SDL_Renderer* renderer, GameState* game);
    void render_bullet(SDL_Renderer* renderer, int x, int y, int size);
    void showStartScreen(SDL_Renderer* renderer);
    int waitForStartGame(SDL_Renderer* renderer);
    void showLobbyScreen(SDL_Renderer* renderer);
};

#endif //SDL_HANDLER_H
