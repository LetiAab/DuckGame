#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>

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

void SDLHandler::loadGame(GameState* game) {
    SDL_Surface* background = loadImage("forest");
    game->background = SDL_CreateTextureFromSurface(game->renderer, background);
    SDL_FreeSurface(background);

    Duck duck{};
    int count = 0;
    game->ducks_quantity = 0;
    for (size_t i = 0; i < game->map->size(); ++i) {
        for (size_t j = 0; j < (*game->map)[i].size(); ++j) {
            if ((*game->map)[i][j] == 1) {
                duck.x = j * TILE_SIZE;
                duck.y = i * TILE_SIZE;
                duck.flipType = SDL_FLIP_NONE;
                count++;
                if (count == 6) {
                    game->ducks[game->ducks_quantity] = duck;
                    game->ducks_quantity++;
                    count=0;
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
    uint8_t move = 0;
    // TODO: ver lo del numero id
    int new_x = game->ducks[id-1].x;
    int new_y = game->ducks[id-1].y;

    if (state[SDL_SCANCODE_A]) {
        new_x -= TILE_SIZE;
        game->ducks[id-1].flipType = SDL_FLIP_HORIZONTAL;
        //msg.type = MOVE_LEFT;
        move = MOVE_LEFT;
        std::cout << "Pato " << id << " se movio a la izquierda\n";
    }
    if (state[SDL_SCANCODE_D]) {
        new_x += TILE_SIZE;
        game->ducks[id-1].flipType = SDL_FLIP_NONE;
        move = MOVE_RIGHT;
        std::cout << "Pato se movio a la derecha\n";
    }
    if (state[SDL_SCANCODE_W]) {
        new_y -= TILE_SIZE;
        move = MOVE_UP;
        std::cout << "Pato se movio para arriba\n";
    }
    if (state[SDL_SCANCODE_S]) {
        new_y += TILE_SIZE;
        move = MOVE_DOWN;
        std::cout << "Pato se movio para abajo\n";
    }

    int new_grid_x = new_x / TILE_SIZE;
    int new_grid_y = new_y / TILE_SIZE;

    if (new_grid_x >= 0 && new_grid_x < static_cast<int>(game->map->at(0).size())
            && new_grid_y >= 0 && new_grid_y < static_cast<int>(game->map->size()) &&
            (*game->map)[new_grid_y][new_grid_x] != 2) {
        game->ducks[id-1].x = new_x;
        game->ducks[id-1].y = new_y;
        positionUpdated = true;
    } else {
        std::cout << "PARED no se puede mover ahi\n";
    }

    if (positionUpdated) {
        auto cmd = Command(id, move, game->ducks[id-1].x, game->ducks[id-1].y);
        game->command_queue->push(cmd);

    }

    return done;
}

void SDLHandler::doRender(SDL_Renderer* renderer, GameState* game) {
    SDL_RenderCopy(renderer, game->background, NULL, NULL);

    for (int i = 0; i < game->ducks_quantity; i++) {
        SDL_Rect duck_rect = {game->ducks[i].x, game->ducks[i].y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopyEx(renderer, game->duck_t, NULL, &duck_rect, 0, NULL, game->ducks[i].flipType);
    }

    for (size_t i = 0; i < game->crates.size(); i++) {
        SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, game->crate, NULL, &crate_rect);
    }

    SDL_RenderPresent(renderer);
}

void SDLHandler::run(std::vector<std::vector<int>> &map, Queue<Command>& command_queue, uint16_t id) {
    GameState game{};
    SDL_Window* window = SDL_CreateWindow("Duck Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          TILE_SIZE*15, TILE_SIZE*10,
                                          0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;

    game.map = &map;
    game.command_queue = &command_queue;
    std::cout << "ID: " << id << "\n";

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