#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <common/message.h>

#define DELAY_TIME 60
#define DUCK_SIZE_X 2 //EN CANTIDAD DE TILE_SIZE
#define DUCK_SIZE_Y 3 //EN CANTIDAD DE TILE_SIZE

#define TILE_SIZE 32

//using namespace SDL2pp;

SDLHandler::SDLHandler() {
    SDL_Init(SDL_INIT_VIDEO);
}

SDLHandler::~SDLHandler() {
    SDL_Quit();
}

SDL_Surface* SDLHandler::loadImage(const std::string& name_img) {
    SDL_Surface* img = NULL;
    const std::string path = std::string(IMAGE_PATH) + name_img + ".png";
    img = IMG_Load(path.c_str());
    if (img == NULL) {
        std::cerr << "Error loading " << name_img << ".png: " << IMG_GetError() << "\n";
        SDL_Quit();
        exit(ERROR);
    }
    return img;
}

void SDLHandler::loadGame(GameState* game) {
    SDL_Surface* background = loadImage("forest");
    game->background = SDL_CreateTextureFromSurface(game->renderer, background);
    SDL_FreeSurface(background);

    Duck duck{};
    int count = 0;
    game->ducks_quantity = 0;
    for (size_t i = 0; i < game->client_game_map.map.size(); ++i) {
        for (size_t j = 0; j < (game->client_game_map.map)[i].size(); ++j) {
            // Verificar si el valor está entre '1' y '6'
            if ((game->client_game_map.map)[i][j] >= '1' && (game->client_game_map.map)[i][j] <= '6') {
                duck.x = j * TILE_SIZE;
                duck.y = i * TILE_SIZE;
                duck.flipType = SDL_FLIP_NONE;
                count++;
                if (count == 6) {
                    game->ducks[game->ducks_quantity] = duck;
                    game->ducks_quantity++;
                    count = 0;
                }
            }
        }
    }

    std::cout << "Cantidad de patos: " << game->ducks_quantity << "\n";

    SDL_Surface* duck_surface = loadImage("duck");
    game->duck_t = SDL_CreateTextureFromSurface(game->renderer, duck_surface);
    SDL_FreeSurface(duck_surface);

    SDL_Surface* crate_surface = loadImage("crate");
    game->crate = SDL_CreateTextureFromSurface(game->renderer, crate_surface);
    SDL_FreeSurface(crate_surface);

    for (size_t i = 0; i < game->client_game_map.map.size(); ++i) {
        for (size_t j = 0; j < (game->client_game_map.map)[i].size(); ++j) {
            if ((game->client_game_map.map)[i][j] == 'P') {
                Crate crate;
                crate.x = j * TILE_SIZE;
                crate.y = i * TILE_SIZE;
                game->crates.push_back(crate);
            }
        }
    }
}

int SDLHandler::processEvents(SDL_Window* window, GameState* game, uint16_t id) {
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

            case SDL_KEYDOWN:  // Evento cuando se presiona una tecla
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        done = ERROR;
                        break;
                    case SDLK_a:  // Izquierda
                        //game->ducks[id-1].flipType = SDL_FLIP_HORIZONTAL;
                        move = MOVE_LEFT;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " se movió a la izquierda\n";
                        break;
                    case SDLK_d:  // Derecha
                        //game->ducks[id-1].flipType = SDL_FLIP_NONE;
                        move = MOVE_RIGHT;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " se movió a la derecha\n";
                        break;
                    case SDLK_w:  // Arriba
                        move = MOVE_UP;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " se movió para arriba\n";
                        break;
                    case SDLK_s:  // Abajo
                        move = MOVE_DOWN;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " se movió para abajo\n";
                        break;
                    default:
                        break;
                }
                break;

            case SDL_KEYUP:  // Evento cuando se suelta una tecla
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                        move = STOP_LEFT;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " dejó de moverse a la izquierda\n";
                        break;
                    case SDLK_d:
                        move = STOP_RIGHT;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " dejó de moverse a la derecha\n";
                        break;
                    case SDLK_w:
                        move = STOP_UP;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " dejo de moverse para arriba\n";
                        break;
                    case SDLK_s:
                        move = STOP_DOWN;
                        positionUpdated = true;
                        std::cout << "Pato " << id << " dejo de moverse para abajo\n";
                        break;
                    default:
                        break;
                }
                break;

            case SDL_QUIT:
                done = ERROR;
                break;

            default:
                break;
        }
    }

    // Envía el comando solo si hubo una actualización de posición o estado
    if (positionUpdated) {
        auto cmd = Command(id, move);
        game->command_queue->push(cmd);
    }

    return done;
}






/*
int SDLHandler::processEvents(SDL_Window* window, GameState* game, uint16_t id) {
    int done = SUCCESS;
    bool positionUpdated = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                if (window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = ERROR;
                }
            break;
            case SDL_KEYUP:
                std::cout << "Key released\n";
            positionUpdated = true;
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        done = ERROR;
                    break;
                    default: break;
                }
            break;
            case SDL_QUIT:
                done = ERROR;
            break;
            default: break;
        }
    }
    const uint8_t* state = SDL_GetKeyboardState(NULL);
    uint8_t move = 0;
    // TODO: ver lo del numero id
    int new_x = game->ducks[id-1].x;
    int new_y = game->ducks[id-1].y;

    if (state[SDL_SCANCODE_A]) {
        new_x -= TILE_SIZE;
        game->ducks[id-1].flipType = SDL_FLIP_HORIZONTAL;
        move = MOVE_LEFT;
        positionUpdated = true;

        std::cout << "Pato " << id << " se movio a la izquierda\n";
    }
    if (state[SDL_SCANCODE_D]) {
        new_x += TILE_SIZE;
        game->ducks[id-1].flipType = SDL_FLIP_NONE;
        move = MOVE_RIGHT;
        positionUpdated = true;

        std::cout << "Pato se movio a la derecha\n";
    }
    if (state[SDL_SCANCODE_W]) {
        new_y -= TILE_SIZE;
        move = MOVE_UP;
        positionUpdated = true;

        std::cout << "Pato se movio para arriba\n";
    }
    if (state[SDL_SCANCODE_S]) {
        new_y += TILE_SIZE;
        move = MOVE_DOWN;
        positionUpdated = true;

        std::cout << "Pato se movio para abajo\n";
    }

  
    if (positionUpdated) {
        auto cmd = Command(id, move, game->ducks[id-1].x, game->ducks[id-1].y);
        game->command_queue->push(cmd);
    }

    return done;
}*/

void SDLHandler::doRender(SDL_Renderer* renderer, GameState* game) {
    
    SDL_RenderCopy(renderer, game->background, NULL, NULL);

    for (int i = 0; i < game->ducks_quantity; i++) {
        std::cout << "RENDERIZO A LOS PATOS" << "\n";
        SDL_Rect duck_rect =
            {game->ducks[i].x, game->ducks[i].y,
            TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y};
            
        SDL_RenderCopyEx(renderer, game->duck_t, NULL, &duck_rect, 0, NULL, game->ducks[i].flipType);
    }

    for (size_t i = 0; i < game->crates.size(); i++) {
        SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, game->crate, NULL, &crate_rect);
    }

    SDL_RenderPresent(renderer);
}

void SDLHandler::run(std::vector<std::vector<char>> &map, Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue) {
    GameState game{};
    SDL_Window* window = SDL_CreateWindow("Duck Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          TILE_SIZE*MATRIX_M, TILE_SIZE*MATRIX_N,
                                          0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;

    game.client_game_map.setMap(map);
    game.command_queue = &command_queue;
    std::cout << "ID: " << id << "\n";

    loadGame(&game);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    while (!done) {
        //PRIMERO MANDO AL SERVER
        done = processEvents(window, &game, id);

        //LUEGO RECIBO DEL SERVER Y HAGO EL RENDER
        Message message;
        message_queue.try_pop(message);
        //TODO: MODULARIZAR
        
        
        if (message.type == DUCK_POS_UPDATE){

            //de el lado de el cliente no importa tanto la matriz. porque es para ver las coliciones
            // y eso se hace en el server.
            //si a mi me llega la posicion del pato simplemente tengo que mover al pato a donte tien que ir

            std::cout << "ME LLEGO NOTIFICACION PARA ACTUALIZAR LA POS DEL PATO" << "\n";

            std::cout << "x " << message.duck_x <<"\n";
            std::cout << "x " << message.duck_y <<"\n";

            int pos_id = message.player_id - 1;

            std::cout << "POS_ID DEL PATO A MOVER: " << pos_id << "\n";

            game.ducks[pos_id].x = message.duck_x * TILE_SIZE;
            game.ducks[pos_id].y = message.duck_y * TILE_SIZE;

            //con looking podemos hacer que el pato mire para arriba o aletee tambien (creo)
            if(message.looking == LOOKING_LEFT){
                //pato esta mirando a la izquierda
                game.ducks[pos_id].flipType = SDL_FLIP_HORIZONTAL;
                
            } else {
                //pato esta mirando a la derecha
                game.ducks[pos_id].flipType = SDL_FLIP_NONE;
            }


        }

        //game.client_game_map.printMap();

        doRender(renderer, &game);
        SDL_Delay(DELAY_TIME);
    }

    // Termino el juego => libero recursos
    SDL_DestroyTexture(game.crate);
    SDL_DestroyTexture(game.duck_t);
    SDL_DestroyTexture(game.background);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}