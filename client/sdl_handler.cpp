#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <common/message.h>

#define DELAY_TIME 60

//using namespace SDL2pp;

SDLHandler::SDLHandler(): handle_textures(nullptr) {
    SDL_Init(SDL_INIT_VIDEO);
}

SDLHandler::~SDLHandler() {
    SDL_Quit();
}

//** Juego **//
void SDLHandler::loadGame(GameState* game) {
    std::vector<TextureInfo> textures_to_load = {
        {"forest", "backgrounds/forest", 1},
        {"crate", "crate", 1},
        {"gun", "guns/AK-47", 1},
        {"bullet", "ammo/bullet", 1},
        {"duck-walking", "duck-walking", 6},
        {"duck-walking-wings", "duck-walking-wings", 1},
        {"duck-jumping", "duck-jumping", 6}
    };

    // Cargo las texturas
    int frame_width = 0, frame_height = 0;
    for (const TextureInfo& texture_info : textures_to_load) {
        handle_textures.loadTexture(texture_info, &frame_width, &frame_height);
    }

    // Inicializo los patos y los crates
    gameInitializer.initializeDucks(game, frame_width, frame_height);
    gameInitializer.initializeCrates(game);

    // Inicializo el render manager
    rendererManager = std::make_unique<RendererManager>(game->renderer, handle_textures);
}

//** Lobby **//
void SDLHandler::showStartScreen(SDL_Renderer* renderer) {
    SDL_Delay(1000);
    SDL_Texture* start_logo = handle_textures.loadSimpleTexture("start/duckgame_logo");
    SDL_Point size;
    SDL_QueryTexture(start_logo, NULL, NULL, &size.x, &size.y);
    SDL_Rect start_logo_rect = {WINDOW_WIDTH/2-size.x/2, WINDOW_HEIGHT/2-size.y/2, size.x, size.y};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, start_logo, NULL, &start_logo_rect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);
    // Fade out effect
    for (int alpha = 255; alpha >= 0; alpha -= 5) {
        SDL_SetTextureAlphaMod(start_logo, alpha);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, start_logo, NULL, &start_logo_rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(start_logo);
}

void SDLHandler::showLobbyScreen(SDL_Renderer *renderer) {
    SDL_Texture* start_background = handle_textures.loadSimpleTexture("start/galaxy");
    SDL_RenderCopy(renderer, start_background, NULL, NULL);

    SDL_Texture* textureText = handle_textures.getText("Lobby", {255, 255, 255, 255});

    int size_x = 400, size_y = 100;
    SDL_Rect textRect = {WINDOW_WIDTH/2-size_x/2, 20, size_x, size_y};
    SDL_RenderCopy(renderer, textureText, NULL, &textRect);

    SDL_Texture* start_button = handle_textures.loadSimpleTexture("start/play");
    SDL_Rect start_button_rect = {WINDOW_WIDTH/2-100, WINDOW_HEIGHT/2-50, 200, 100};
    SDL_RenderCopy(renderer, start_button, NULL, &start_button_rect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(start_background);
    SDL_DestroyTexture(textureText);
}

int SDLHandler::waitForStartGame(SDL_Renderer* renderer) {
    int done = SUCCESS;
    bool start_game = false;

    while (!start_game && !done) {
        showLobbyScreen(renderer);
        done = eventProcessor.processLobbyEvents(start_game);
    }
    return done;
}

void SDLHandler::run(std::vector<std::vector<char>> &map, Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue) {
    SDL_Window* window = SDL_CreateWindow("Duck Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    handle_textures = TextureHandler(renderer);

    showStartScreen(renderer);

    if(waitForStartGame(renderer) == ERROR) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return;
    }

    GameState game{};
    game.renderer = renderer;
    game.client_game_map.setMap(map);
    game.command_queue = &command_queue;
    std::cout << "ID: " << id << "\n";

    loadGame(&game);
    rendererManager->doRenderStatic(&game);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    while (!done) {
        //PRIMERO MANDO AL SERVER
        done = eventProcessor.processGameEvents(window, &game, id);

        //LUEGO RECIBO DEL SERVER Y HAGO EL RENDER
        Message message;
        message_queue.try_pop(message);
        //TODO: MODULARIZAR
        
        if(message.type == BULLET_POS_UPDATE){

            //renderizar la bala. NO se como ahcerlo aca y no dentro del DORender.
            //capaz simplemente va ahi

            //cada bala viene con su id para poder identificarla de alguna forma al momento de renderizar
        }

        if(message.type == KILL_DUCK){
            int pos_id = message.player_id - 1;
            //NO LO ELIMINO DE LA LISTA PORQUE ALTO KILOMBO ASI QUE MUEVO EL DIBUJO AFUERA DE LA PANTALLA
            game.ducks[pos_id].x = 400 * TILE_SIZE;
            game.ducks[pos_id].y = 400 * TILE_SIZE;
        }

        if (message.type == DUCK_POS_UPDATE){


            int pos_id = message.player_id - 1;

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

            game.ducks[pos_id].is_moving = message.is_moving;
            
            //intente hacer estas animaciones y fallé :(
            game.ducks[pos_id].is_jumping = message.is_jumping;
            game.ducks[pos_id].is_fluttering = message.is_fluttering;

        }

        rendererManager->doRenderDynamic(&game, message);

        SDL_Delay(DELAY_TIME);
    }

    // Termino el juego => libero recursos
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}