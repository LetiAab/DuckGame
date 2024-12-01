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
            case SDLK_j:
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
            case SDLK_k:
                move = LOOK_UP;
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
            case SDLK_k:
                move = STOP_LOOK_UP;
                break;
            default:
                break;
        }
    }
    return move;
}

int EventProcessor::processLobbyEvents(ScreenManager* screenManager, Queue<Command>& command_queue, uint16_t id, bool& is_alive, int& chosen_match, bool& selected_match) {
    int done = SUCCESS;
    SDL_Event event;
    int x, y;

    try {
        Button *start = screenManager->getButton(START_MATCH_CODE);
        Button *new_match = screenManager->getButton(NEW_MATCH_CODE);
        Button *list_matches = screenManager->getButton(LIST_MATCH_AVAILABLE);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT_CLOSE:
                    is_alive = false;
                    if (command_queue.try_push(Command(id, LOBBY_EXIT_CODE))){
                        std::cout << "Escape!" << "\n";
                        command_queue.close();
                    };
                    //command_queue.close();
                    /*command_queue.push(Command(id, LOBBY_EXIT_CODE));
                    done = ERROR;*/
                break;
                case SDL_MOUSEBUTTONDOWN: {
                    std::cout << "Mouse PRESSED\n";
                    x = event.button.x;
                    y = event.button.y;
                    std::cout << "X: " << x << " Y: " << y << "\n";

                    if (x >= start->x && x <= start->x+BUTTON_W && y >= start->y && y <= start->y+BUTTON_H) {
                        if (command_queue.try_push(Command(id, START_MATCH_CODE, chosen_match))){
                            std::cout << "Iniciando partida..." << "\n";
                        }
                        //is_alive = false;
                        std::cout << "Empezo el juego!\n";


                    } else if (x >= new_match->x && x <= new_match->x+BUTTON_W && y >= new_match->y && y <= new_match->y+BUTTON_H) {
                        //selected_match = false;
                        // si se presiona se crea una nueva partida => se escribe el id de la partida nueva
                        if (command_queue.try_push(Command(id, NEW_MATCH_CODE, chosen_match))){
                            std::cout << "Creando partida..." << "\n";
                        }
                    } else if (x >= list_matches->x && x <= list_matches->x+BUTTON_W && y >= list_matches->y && y <= list_matches->y+BUTTON_H) {
                        // list matches
                        // aparece un menu con las partidas disponibles
                        // luego se puede presionar una de las partidas
                        std::cout << "Ver listado de partidas\n";
                        if (command_queue.try_push(Command(id, LIST_MATCH_AVAILABLE))){
                            std::cout << "Partidas disponibles..." << "\n";
                        };
                        selected_match = true;
                        std::cout << "Seleccionando... " << selected_match << "\n";
                    } else if (selected_match && x >= 504 && x<= WINDOW_WIDTH && y >= 269 && y <= 446) {
                        // join match
                        // si se presiona se conecta a la partida seleccionada
                        screenManager->renderSelectedMatch(x, y, chosen_match);
                        std::cout << "Partida seleccionada: " << chosen_match << "\n";
                        if (command_queue.try_push(Command(id, EXISTING_MATCH_CODE, chosen_match))){
                            std::cout << "Conectando a partida..." << "\n";
                        };
                    }
                break;
                }
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        auto stop_command = Command(id, LOBBY_EXIT_CODE);
                        if (command_queue.try_push(stop_command)){
                            std::cout << "Escape!" << "\n";
                            command_queue.close();
                        }
                        is_alive = false;
                    }
                break;
                case SDL_QUIT:
                    is_alive = false;
                    command_queue.close();
                break;
                default:
                    break;
            }
        }

    } catch (std::runtime_error& e) {
        done = ERROR;
        is_alive = false;
        std::cerr << e.what() << "\n";
    }
    return done;
}