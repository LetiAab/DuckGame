#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include "common/constants.h"

#define DELAY_TIME 50
#define TILE_SIZE 60

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

// TODO: Segun el tipo de objeto, se lo ubica en la posicion correspondiente
// 1 -> duck, 2 -> crate, etc
/*void SDLHandler::locateObject(GameState* game, int& num) {
    for (size_t i = 0; i < game->map->size(); ++i) {
        for (size_t j = 0; j < (*game->map)[i].size(); ++j) {
            if ((*game->map)[i][j] == num) {
                Object obj;
                obj.x = j * TILE_SIZE;
                obj.y = i * TILE_SIZE;
                game->objects.push_back(obj);
            }
        }
    }
}*/

void SDLHandler::loadGame(GameState* game) {
    SDL_Surface* background = loadImage("forest");
    game->background = SDL_CreateTextureFromSurface(game->renderer, background);
    SDL_FreeSurface(background);

    bool foundDuck = false;
    for (size_t i = 0; i < game->map->size() && !foundDuck; ++i) {
        for (size_t j = 0; j < (*game->map)[i].size() && !foundDuck; ++j) {
            if ((*game->map)[i][j] == 1) {
                game->duck.x = j * TILE_SIZE;
                game->duck.y = i * TILE_SIZE;
                foundDuck = true;
            }
        }
    }
    game->duck.flipType = SDL_FLIP_NONE;
    SDL_Surface* duck_surface = loadImage("duck");
    game->duck_t = SDL_CreateTextureFromSurface(game->renderer, duck_surface);
    SDL_FreeSurface(duck_surface);

    SDL_Surface* crate_surface = loadImage("crate");
    game->crate = SDL_CreateTextureFromSurface(game->renderer, crate_surface);
    SDL_FreeSurface(crate_surface);

    for (size_t i = 0; i < game->map->size(); ++i) {
        for (size_t j = 0; j < (*game->map)[i].size(); ++j) {
            if ((*game->map)[i][j] == 2) {
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
    Message msg;
    msg.player_id = id;
    int new_x = game->duck.x;
    int new_y = game->duck.y;

    if (state[SDL_SCANCODE_A]) {
        new_x -= TILE_SIZE;
        game->duck.flipType = SDL_FLIP_HORIZONTAL;
        msg.type = MOVE_LEFT;
        std::cout << "Pato se movio a la izquierda\n";
    }
    if (state[SDL_SCANCODE_D]) {
        new_x += TILE_SIZE;
        game->duck.flipType = SDL_FLIP_NONE;
        msg.type = MOVE_RIGHT;
        std::cout << "Pato se movio a la derecha\n";
    }
    if (state[SDL_SCANCODE_W]) {
        new_y -= TILE_SIZE;
        msg.type = MOVE_UP;
        std::cout << "Pato se movio para arriba\n";
    }
    if (state[SDL_SCANCODE_S]) {
        new_y += TILE_SIZE;
        msg.type = MOVE_DOWN;
        std::cout << "Pato se movio para abajo\n";
    }

    int new_grid_x = new_x / TILE_SIZE;
    int new_grid_y = new_y / TILE_SIZE;

    if (new_grid_x >= 0 && new_grid_x < static_cast<int>(game->map->at(0).size())
            && new_grid_y >= 0 && new_grid_y < static_cast<int>(game->map->size()) &&
            (*game->map)[new_grid_y][new_grid_x] != 2) {
        game->duck.x = new_x;
        game->duck.y = new_y;
        positionUpdated = true;
    } else {
        std::cout << "PARED no se puede mover ahi\n";
    }

    if (positionUpdated) {
        msg.duck_x = game->duck.x;
        msg.duck_y = game->duck.y;
        game->message_queue->push(msg);
    }

    return done;
}

void SDLHandler::doRender(SDL_Renderer* renderer, GameState* game) {
    SDL_RenderCopy(renderer, game->background, NULL, NULL);

    SDL_Rect duck_rect = {game->duck.x, game->duck.y, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopyEx(renderer, game->duck_t, NULL, &duck_rect, 0, NULL, game->duck.flipType);

    for (size_t i = 0; i < game->crates.size(); i++) {
        SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, game->crate, NULL, &crate_rect);
    }

    SDL_RenderPresent(renderer);
}

void SDLHandler::run(Queue<Message>& message_queue) {
    GameState game{};
    SDL_Window* window = SDL_CreateWindow("Duck Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          TILE_SIZE*15, TILE_SIZE*10,
                                          0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;

    Message first_message = message_queue.pop();
    game.map = &first_message.map;
    game.message_queue = &message_queue;
    uint16_t id = first_message.player_id;

    loadGame(&game);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    while (!done) {
        done = processEvents(window, &game, id);
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