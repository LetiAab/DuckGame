#include "sdl_eventprocessor.h"

int EventProcessor::processGameEvents(SDL_Window* window, GameState* game, uint16_t id) {
    int done = SUCCESS;
    bool positionUpdated = false;
    SDL_Event event;
    uint8_t move = 0;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                if (window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = ERROR;
                }
                break;
            case SDL_KEYDOWN:
                move = handleKeyDown(event.key.keysym.sym);
                if (move == ERROR) {
                    done = ERROR;
                    break;
                }
                if (move != 0) {
                    positionUpdated = true;
                }
                break;
            case SDL_KEYUP:
                move = handleKeyUp(event.key.keysym.sym);
                if (move != 0) {
                    positionUpdated = true;
                }
                break;
            case SDL_QUIT:
                done = ERROR;
                break;
            default:
                break;
        }
    }

    if (positionUpdated) {
        auto cmd = Command(id, move);
        game->command_queue->push(cmd);
    }

    return done;
}

uint8_t EventProcessor::handleKeyDown(SDL_Keycode key) {
    uint8_t move = 0;
    // solo si la tecla no estaba ya presionada
    if(!keyState[key]) {
        keyState[key] = true;
        switch (key) {
            case SDLK_ESCAPE:
                move = ERROR;
                break;
            case SDLK_a:
                move = MOVE_LEFT;
                break;
            case SDLK_d:
                move =  MOVE_RIGHT;
                break;
            case SDLK_w:
                move =  MOVE_UP;
                break;
            case SDLK_s:
                move =  MOVE_DOWN;
                break;
            case SDLK_f:
                move =  SHOOT;
                break;
            default:
                break;
        }
    }
    return move;
}

uint8_t EventProcessor::handleKeyUp(SDL_Keycode key) {
    uint8_t move = 0;
    //solo si la tecla estaba presionada
    if (keyState[key]) {
        keyState[key] = false;
        switch (key) {
            case SDLK_a:
                move = STOP_LEFT;
                break;
            case SDLK_d:
                move = STOP_RIGHT;
                break;
            case SDLK_w:
                move = STOP_UP;
                break;
            case SDLK_s:
                move = STOP_DOWN;
                break;
            default:
                break;
        }
    }
    return move;
}
