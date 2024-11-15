#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <common/message.h>

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
    handle_textures = SDLHandleTextures(game->renderer);
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
                            break;
                        case SDLK_e:
                            move = TAKE_ITEM;
                            positionUpdated = true;
                            break;

                        case SDLK_q:
                            move = DROP_WEAPON;
                            positionUpdated = true;
                            break;
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

// ESTO LO PUSE (MATEO) DE FORMA AUXILIAR PARA VER LOS ITEMS PERO SE ESTAN RENDERIZANDO
// UNA VEZ POR LOOP Y SON ESTATICOS EN REALIDAD
void SDLHandler::renderItems(SDL_Renderer* renderer, GameState* game) {

    for (size_t i = 0; i < game->items.size(); i++) {

        SDL_Rect item_rect = {game->items[i].x, game->items[i].y, TILE_SIZE * 4, TILE_SIZE * 4};

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RGB: rojo, opacidad completa

        SDL_RenderFillRect(renderer, &item_rect);
    }

    SDL_RenderPresent(renderer);
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

        //TODO: MODULARIZAr
        if(message.type == ITEM_POSITION){

            //RENDERIZAR LOS ITEMS bien 
            Item item;
            item.x = message.item_x * TILE_SIZE;
            item.y = message.item_y * TILE_SIZE;
            game.items.push_back(item);
        }

        if(message.type == DUCK_PICKUP_ITEM){
            
            std::cout << "ME PONGO EL ITEM EN LA MANO" << "\n";
            //aca te fijas en el mensaje el DUCK ID y el ITEM ID.
            //cuando recibis esto tenes que renderizarle al pato con DUCK ID el ITEM ID 
            //EN LA MANO. OSEA TODABIA NO LO TIENE PUESTO. con las armas es lo mismo
            //LA DIFERENCIA ES CUANDO AGARRO UNA ARMADURA O CASCO
        }

        if(message.type == DUCK_EQUIP_ITEM){

            std::cout << "ME EQUIPO EL ITEM QUE TENGO EN LA MANO" << "\n";

            //SI RECIBO ESTO AHORA SI ME TENGO QUE PONER LA ARMADURA O EMPUÑAR EL ARMA
        }

        if(message.type == DROP_WEAPON){
            std::cout << "SUELTO EL ARMA" << "\n";
        }

        if(message.type == ARMOR_BROKEN){
            std::cout << "SE ME ROMPIO EL ARMOR" << "\n";
        }

        if(message.type == HELMET_BROKEN){
            std::cout << "SE ME ROMPIO EL HELMET" << "\n";
        }




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

        //REVISAR ESTO NO CREO QUE ESTÉ BIEN RENDERIZARLO EN CADA LOOP
        renderItems(renderer, &game);

        SDL_Delay(DELAY_TIME);
    }

    // Termino el juego => libero recursos
    handle_textures.destroyTextures();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}