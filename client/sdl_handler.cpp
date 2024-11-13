#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <common/message.h>
#include <SDL2/SDL_ttf.h>

#define DELAY_TIME 60
//#define DUCK_SIZE_X 2 //EN CANTIDAD DE TILE_SIZE
//#define DUCK_SIZE_Y 3 //EN CANTIDAD DE TILE_SIZE
//#define TILE_SIZE 2

//using namespace SDL2pp;

SDLHandler::SDLHandler(): handle_textures(SDLHandleTextures(nullptr)) {
    SDL_Init(SDL_INIT_VIDEO);
}

SDLHandler::~SDLHandler() {
    SDL_Quit();
}

void SDLHandler::loadGame(GameState* game) {
    // Cargo las texturas
    //handle_textures = SDLHandleTextures(game->renderer);
    SDL_Texture *background = handle_textures.loadSimpleTexture("backgrounds/forest");
    SDL_Texture *crate_t = handle_textures.loadSimpleTexture("crate");
    SDL_Texture *gun = handle_textures.loadSimpleTexture("guns/AK-47");
    SDL_Texture *bullet = handle_textures.loadSimpleTexture("ammo/bullet");

    SpriteSheet sp = handle_textures.loadSpriteSheet("duck-walking");
    SDL_Texture *walk_duck = sp.texture;
    int frame_width = sp.frame_width;
    int frame_height = sp.frame_height;
    SDL_Texture *walk_wings = handle_textures.loadSimpleTexture("duck-walking-wings");

    SpriteSheet sp_jump = handle_textures.loadSpriteSheet("duck-jumping");
    SDL_Texture *jump_duck = sp_jump.texture;

    handle_textures.saveTexture("background", background);
    handle_textures.saveTexture("duck-walking", walk_duck);
    handle_textures.saveTexture("duck-jumping", jump_duck);
    handle_textures.saveTexture("duck-walking-wings", walk_wings);
    handle_textures.saveTexture("crate", crate_t);
    handle_textures.saveTexture("gun", gun);
    handle_textures.saveTexture("bullet", bullet);

    // Inicializo los patos y los crates
    initializeDucks(game, frame_width, frame_height);
    initializeCrates(game);

}

void SDLHandler::initializeDucks(GameState* game, const int frame_width, const int frame_height) {
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
                duck.is_moving = false;
                duck.animation_frame = 0;
                duck.current_frame_index = 0;

                // Configuro el tamaño de los fotogramas del pato
                duck.frame_width = frame_width;
                duck.frame_height = frame_height;

                count++;
                if (count == DUCK_TOTAL_SIZE) {
                    game->ducks[game->ducks_quantity] = duck;
                    game->ducks_quantity++;
                    count = 0;
                }
            }
        }
    }
    std::cout << "Cantidad de patos: " << game->ducks_quantity << "\n";
}

void SDLHandler::initializeCrates(GameState* game) {
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

            case SDL_KEYDOWN:
                if (!keyState[event.key.keysym.sym]) {// solo si la tecla no estaba ya presionada
                    keyState[event.key.keysym.sym] = true;
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            done = ERROR;
                            break;
                        case SDLK_a:
                            move = MOVE_LEFT;
                            positionUpdated = true;
                            break;
                        case SDLK_d:
                            move = MOVE_RIGHT;
                            positionUpdated = true;
                            break;
                        case SDLK_w:
                            move = MOVE_UP;
                            positionUpdated = true;
                            break;
                        case SDLK_s:
                            move = MOVE_DOWN;
                            positionUpdated = true;
                            break;
                        case SDLK_f:
                            move = SHOOT;
                            positionUpdated = true;
                        default:
                            break;
                    }
                }
                break;

            case SDL_KEYUP:
                if (keyState[event.key.keysym.sym]) {//solo si la tecla estaba presionada
                    keyState[event.key.keysym.sym] = false;
                    switch (event.key.keysym.sym) {
                        case SDLK_a:
                            move = STOP_LEFT;
                            positionUpdated = true;
                            break;
                        case SDLK_d:
                            move = STOP_RIGHT;
                            positionUpdated = true;
                            break;
                        case SDLK_w:
                            move = STOP_UP;
                            positionUpdated = true;
                            break;
                        case SDLK_s:
                            move = STOP_DOWN;
                            positionUpdated = true;
                            break;

                        //stop shoot?
                        default:
                            break;
                    }
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

void SDLHandler::render_bullet(SDL_Renderer* renderer, int x, int y, int size = 20) {
    SDL_Rect bulletRect = { x * TILE_SIZE, y * TILE_SIZE, size, size };

    /*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &bulletRect);*/

    //SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, TILE_SIZE, TILE_SIZE};
    SDL_RenderCopy(renderer, handle_textures.getTexture("bullet"), NULL, &bulletRect);
}

void SDLHandler::doRenderStatic(SDL_Renderer* renderer, GameState* game) {

    SDL_RenderCopy(renderer, handle_textures.getTexture("background"), NULL, NULL);

    for (size_t i = 0; i < game->crates.size(); i++) {
        SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, handle_textures.getTexture("crate"), NULL, &crate_rect);
    }

    //SDL_RenderPresent(renderer);
}

void SDLHandler::doRenderDynamic(SDL_Renderer* renderer, GameState* game, Message& message) {
    for (int i = 0; i < game->ducks_quantity; i++) {
        Duck& duck = game->ducks[i];
        
        // Cambiar el fotograma de animación si el pato está en movimiento
        if (duck.is_moving) {
            duck.current_frame_index = (duck.current_frame_index + 1) % 6;  // Ciclar entre 6 fotogramas
        } else {
            duck.current_frame_index = 0;  // Si no se mueve, detener la animación
        }

        SDL_Rect src_rect = {
            duck.current_frame_index * duck.frame_width, // Desplazar en el sprite sheet
            0,
            duck.frame_width,
            duck.frame_height
        };

        SDL_Rect duck_rect = {
            duck.x, duck.y,
            TILE_SIZE * DUCK_SIZE_X,
            TILE_SIZE * DUCK_SIZE_Y
        };

        SDL_Rect gun_rect = {
            duck.x,
            duck.y + TILE_SIZE/4,
            TILE_SIZE * DUCK_SIZE_X,
            TILE_SIZE * DUCK_SIZE_Y
        };

        SDL_Texture* duck_texture = handle_textures.getTexture("duck-walking");
        SDL_SetTextureColorMod(duck_texture, colors[i][0], colors[i][1], colors[i][2]);
        SDL_RenderCopyEx(renderer, duck_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
        SDL_SetTextureColorMod(duck_texture, 255, 255, 255); //reseteo el color

        SDL_RenderCopyEx(renderer, handle_textures.getTexture("gun"), NULL, &gun_rect, 0, NULL, duck.flipType);

        SDL_Texture* wings_texture = handle_textures.getTexture("duck-walking-wings");
        SDL_SetTextureColorMod(wings_texture, colors[i][0], colors[i][1], colors[i][2]);
        SDL_RenderCopyEx(renderer, wings_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
        SDL_SetTextureColorMod(wings_texture, 255, 255, 255); //reseteo el color

        
        
    }

    if(message.type == BULLET_POS_UPDATE){
        render_bullet(renderer, message.bullet_x, message.bullet_y);
    }

    SDL_RenderPresent(renderer);
}

void SDLHandler::showLobbyScreen(SDL_Renderer *renderer, TTF_Font* font) {
    SDL_Texture* start_background = handle_textures.loadSimpleTexture("start/galaxy");
    SDL_RenderCopy(renderer, start_background, NULL, NULL);
    //loadFont(renderer);

    // Crear superficie de texto
    SDL_Surface* surfaceText = TTF_RenderText_Solid(font, "Let's play!", {255, 255, 255, 255});

    // Setup texture
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);

    //TILE_SIZE*MATRIX_M/2-size.x/2
    int size_x = 400;
    int size_y = 100;
    SDL_Rect textRect = {TILE_SIZE*MATRIX_M/2-size_x/2, 20, size_x, size_y};

    SDL_RenderCopy(renderer, textureText, NULL, &textRect);

    SDL_Texture* start_button = handle_textures.loadSimpleTexture("start/start_init");
    SDL_Rect start_button_rect = {TILE_SIZE*MATRIX_M/2-100, TILE_SIZE*MATRIX_N/2-50, 200, 100};
    SDL_RenderCopy(renderer, start_button, NULL, &start_button_rect);


    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(start_background);
    SDL_DestroyTexture(textureText);
}

/*void SDLHandler::loadFont(SDL_Renderer *renderer) {
    if(TTF_Init() == -1) {
        std::cout << "Could not initialize SDL2 TTF: " << TTF_GetError() << "\n";
    }


    // Cargar fuente
    TTF_Font* font = TTF_OpenFont("fonts/8bitOperatorPlus8-Regular.ttf", 32);
    if(font == nullptr) {
        std::cout << "Could not load font: " << TTF_GetError() << "\n";
    }

    // Crear superficie de texto
    SDL_Surface* surfaceText = TTF_RenderText_Solid(font, "Duck  probando", {255, 255, 255});

    // Setup texture
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

    SDL_FreeSurface(surfaceText);

    SDL_Rect textRect = {10, 10, 400, 100};

    SDL_RenderCopy(renderer, textureText, NULL, &textRect);

}*/


void SDLHandler::showStartScreen(SDL_Renderer* renderer) {
    SDL_Texture* start_logo = handle_textures.loadSimpleTexture("start/duckgame_logo");
    SDL_Point size;
    SDL_QueryTexture(start_logo, NULL, NULL, &size.x, &size.y);
    SDL_Rect start_logo_rect = {TILE_SIZE*MATRIX_M/2-size.x/2, TILE_SIZE*MATRIX_N/2-size.y/2, size.x, size.y};
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

int SDLHandler::waitForStartGame(SDL_Renderer* renderer, TTF_Font* font) {
    int done = SUCCESS;
    bool start_game = false;
    SDL_Event event;

    int x, y;

    while (!start_game && !done) {
        showLobbyScreen(renderer, font);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT_CLOSE:
                    done = ERROR;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "Mouse PRESS\n";
                    x = event.button.x;
                    y = event.button.y;
                    std::cout << "X: " << x << " Y: " << y << "\n";
                    std::cout << "tamX: " << TILE_SIZE*MATRIX_M/2-100 << " tamY: " << TILE_SIZE*MATRIX_M/2-50 << "\n";
                    //tamX: 300 tamY: 350
                    //TILE_SIZE*MATRIX_M/2-100, TILE_SIZE*MATRIX_M/2-50, 200, 100
                    if (x >= TILE_SIZE*MATRIX_M/2-100 && x <= TILE_SIZE*MATRIX_M/2+100 &&
                        y >= TILE_SIZE*MATRIX_N/2-50 && y <= TILE_SIZE*MATRIX_N/2+50) {
                        start_game = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:
                            start_game = true;
                            break;
                        case SDLK_ESCAPE:
                            done = ERROR;
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
    }
    return done;
}

void SDLHandler::run(std::vector<std::vector<char>> &map, Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue) {
    SDL_Window* window = SDL_CreateWindow("Duck Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          TILE_SIZE*MATRIX_M, TILE_SIZE*MATRIX_N,
                                          0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    handle_textures = SDLHandleTextures(renderer);

    if(TTF_Init() == -1) {
        std::cout << "Could not initialize SDL2 TTF: " << TTF_GetError() << "\n";
    }


    // Cargar fuente
    const std::string path = std::string(FONT_PATH) + "8bitOperatorPlus8-Regular.ttf";
    TTF_Font* font = TTF_OpenFont(path.c_str(), 16);
    if(font == nullptr) {
        std::cout << "Could not load font: " << TTF_GetError() << "\n";
    }

    showStartScreen(renderer);
    if(waitForStartGame(renderer, font) == ERROR){
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

    SDL_Texture* static_scene = handle_textures.createRenderTarget("static_scene", TILE_SIZE * MATRIX_M, TILE_SIZE * MATRIX_N);
    SDL_SetRenderTarget(renderer, static_scene);
    doRenderStatic(renderer, &game);
    SDL_SetRenderTarget(renderer, NULL);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    while (!done) {
        //PRIMERO MANDO AL SERVER
        done = processEvents(window, &game, id);

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

        SDL_RenderCopy(renderer, handle_textures.getTexture("static_scene"), NULL, NULL);
        doRenderDynamic(renderer, &game, message);

        SDL_Delay(DELAY_TIME);
    }

    // Termino el juego => libero recursos
    handle_textures.destroyTextures();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
}