#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include "common/constants.h"

#define DELAY_TIME 30

using namespace SDL2pp;

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

    game->duck.x = 280;
    game->duck.y = 300;
    game->duck.flipType = SDL_FLIP_NONE;
    SDL_Surface* duck_surface = loadImage("duck");
    game->duck_t = SDL_CreateTextureFromSurface(game->renderer, duck_surface);
    SDL_FreeSurface(duck_surface);

    SDL_Surface* crate_surface = loadImage("crate");
    game->crate = SDL_CreateTextureFromSurface(game->renderer, crate_surface);
    SDL_FreeSurface(crate_surface);

    for (int i = 0; i < 4; i++) {
        game->crates[i].x = 400 + i * 50;
        game->crates[i].y = 300;
    }
}

int SDLHandler::processEvents(SDL_Window* window, GameState* game) {
    int done = SUCCESS;
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
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        done = ERROR;
                        break;
                    default: break;
                }
                break;
            case SDL_KEYUP:
                std::cout << "Key released\n";
                break;
            case SDL_QUIT:
                done = ERROR;
                break;
            default: break;
        }
    }

    const uint8_t* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_A]) {
        game->duck.x--;
        game->duck.flipType = SDL_FLIP_HORIZONTAL;
        std::cout << "Pato se movio a la izquierda\n";
    }
    if (state[SDL_SCANCODE_D]) {
        game->duck.x++;
        game->duck.flipType = SDL_FLIP_NONE;
        std::cout << "Pato se movio a la derecha\n";
    }
    if (state[SDL_SCANCODE_W]) {
        game->duck.y--;
        std::cout << "Pato se movio para arriba\n";
    }
    if (state[SDL_SCANCODE_S]) {
        game->duck.y++;
        std::cout << "Pato se movio para abajo\n";
    }
    std::cout << state << "\n";

    return done;
}

void SDLHandler::doRender(SDL_Renderer* renderer, GameState* game) {
    SDL_RenderCopy(renderer, game->background, NULL, NULL);

    SDL_Rect duck_rect = {game->duck.x, game->duck.y, 100, 100};
    SDL_RenderCopyEx(renderer, game->duck_t, NULL, &duck_rect, 0, NULL, game->duck.flipType);

    for (int i = 0; i < 4; i++) {
        SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, 100, 100};
        SDL_RenderCopy(renderer, game->crate, NULL, &crate_rect);
    }

    SDL_RenderPresent(renderer);
}

void SDLHandler::run() {
    GameState game{};
    SDL_Window* window = SDL_CreateWindow("Duck Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    game.renderer = renderer;

    loadGame(&game);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    while (!done) {
        done = processEvents(window, &game);
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