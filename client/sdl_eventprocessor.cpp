#include "sdl_eventprocessor.h"

#include <iostream>

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
            case SDLK_e:
                move = TAKE_ITEM;
                break;
            case SDLK_q:
                move = DROP_WEAPON;
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

int EventProcessor::processLobbyEvents(bool& start_game) {
    int done = SUCCESS;
    SDL_Event event;
    int x, y;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                done = ERROR;
                break;
            case SDL_MOUSEBUTTONDOWN:
                std::cout << "Mouse PRESSED\n";
                x = event.button.x;
                y = event.button.y;
                std::cout << "X: " << x << " Y: " << y << "\n";
                std::cout << "tamX: " << WINDOW_WIDTH/2-100 << " tamY: " << WINDOW_HEIGHT/2-50 << "\n";
                //tamX: 300 tamY: 350
                //TILE_SIZE*MATRIX_M/2-100, TILE_SIZE*MATRIX_M/2-50, 200, 100
                if (x >= WINDOW_WIDTH/2-100 && x <= WINDOW_WIDTH/2+100 &&
                    y >= WINDOW_HEIGHT/2-50 && y <= WINDOW_HEIGHT/2+50) {
                    start_game = true;
                    }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    done = ERROR;
                }
                break;
            case SDL_QUIT:
                done = ERROR;
                break;
            default:
                break;
        }
    }
    return done;
}