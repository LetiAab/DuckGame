#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include "client_protocol.h"
#include "game_initializer.h"
#include "common/message.h"
#include "common/liberror.h"
#include "sdl_texturehandler.h"
#include "game_state.h"
#include "sdl_eventprocessor.h"
#include "sdl_renderermanager.h"
#include "sdl_screenmanager.h"
#include "sdl_audiomanager.h"
#include <map>


class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    int run(uint16_t lobby_id, Queue<Command>& command_queue, Queue<Message>& message_queue, ClientProtocol& protocol);

private:
    std::map<uint8_t, int> weaponAmmo = {
        {PEW_PEW_LASER_ID, 12},
        {LASER_RIFLE_ID, 10},
        {AK_47_ID, 30},
        {DUEL_PISTOL_ID, 1},
        {COWBOY_PISTOL_ID, 6},
        {MAGNUM_ID, 6},
        {SHOTGUN_ID, 2},
        {SNIPER_ID, 3}
    };

    bool is_alive;
    TextureHandler handle_textures;
    EventProcessor eventProcessor;
    GameInitializer gameInitializer;
    std::unique_ptr<ScreenManager> screenManager;
    std::unique_ptr<RendererManager> rendererManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<Camera> camera;
    uint16_t duck_id;
    bool lobby_exit;
    void loadGame(GameState &game, Queue<Message> &message_queue);
    Message handleMessages(GameState* game, Queue<Message>& message_queue);
    //ClientProtocol& protocol
    int waitForStartGame(uint16_t lobby_id, Queue<Command>& command_queue, Queue<Message>& message_queue, ClientProtocol& protocol);
    void initializeWindow(SDL_Window*& window, SDL_Renderer*& renderer);
    int runGame(SDL_Window* window, SDL_Renderer* renderer, Queue<Command>& command_queue, Queue<Message>& message_queue);

};

#endif //SDL_HANDLER_H
