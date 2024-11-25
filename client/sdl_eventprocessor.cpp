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

                if(move == MUTE){
                    std::cout << "CAMBIO EL MUTE" << "\n";
                    game->music = !game->music;
                }
                if (move != 0 && move != MUTE) {
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
            case SDLK_m:
                move = MUTE;
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

int EventProcessor::processLobbyEvents(ScreenManager* screenManager, bool& start_game, int& id_match) {
    int done = SUCCESS;
    SDL_Event event;
    int x, y;
    try {
        Button *start = screenManager->getButton(START_MATCH_CODE);
        Button *new_match = screenManager->getButton(NEW_MATCH_CODE);
        Button *join_match = screenManager->getButton(LIST_MATCH_AVAILABLE);

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

                if (x >= start->x && x <= start->x+BUTTON_W && y >= start->y && y <= start->y+BUTTON_H) {
                    start_game = true;
                    std::cout << "Empezo el juego!\n";
                }
                // new match
                // si se presiona se crea una nueva partida => se escribe el id de la partida nueva
                if (x >= new_match->x && x <= new_match->x+BUTTON_W && y >= new_match->y && y <= new_match->y+BUTTON_H) {
                    screenManager->renderNewMatchText(id_match);
                    std::cout << "Nueva partida creada con id: " << id_match << "\n";
                    id_match++;
                }
                // join match
                // aparece un menu con las partidas disponibles
                // luego se puede presionar una de las partidas
                if (x >= join_match->x && x <= join_match->x+BUTTON_W && y >= join_match->y && y <= join_match->y+BUTTON_H) {
                    screenManager->renderAvailableMatches(id_match);
                    std::cout << "Ver listado de partidas\n";
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

    } catch (std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }
    return done;
}