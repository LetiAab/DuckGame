#ifndef SDL_EVENTPROCESSOR_H
#define SDL_EVENTPROCESSOR_H
#include <SDL2/SDL.h>
#include <SDL_keycode.h>
#include <unordered_map>
#include "game_state.h"
#include "sdl_screenmanager.h"
#include "sdl_audiomanager.h"

class EventProcessor {
public:
    EventProcessor() = default;
    int processGameEvents(SDL_Window *window, GameState *game, uint16_t id);
    int processLobbyEvents(ScreenManager* screenManager, Queue<Command>& command_queue, uint16_t id, bool& is_alive, int& chosen_match, bool& selected_match, AudioManager* audioManager);
private:
    std::unordered_map<SDL_Keycode, bool> keyState; //necesito esto para que se mande un solo comando
    uint8_t handleKeyDown(SDL_Keycode key);
    uint8_t handleKeyUp(SDL_Keycode key);

};

#endif //SDL_EVENTPROCESSOR_H
