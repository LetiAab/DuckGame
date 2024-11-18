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
        {"spawn", "spawn-place", 1},
        {"helmet", "armor/helmet", 1},
        {"armor", "armor/armor", 1},
        {"bullet", "ammo/bullet", 1},
        {"duck-walking", "duck-walking", 6},
        {"duck-walking-wings", "duck-walking-wings", 1},
        {"duck-jumping", "duck-jumping", 1},
        {"duck-jumping-wings", "duck-jumping-wings", 1},
        {"duck-fluttering", "duck-fluttering", 6}
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



      
Message SDLHandler::handleMessages(GameState *game, Queue<Message> &message_queue) {
    Message message;
    while (message_queue.try_pop(message)) {

        if(message.type == END_GAME){
            std::cout << "SE TERMINO LA PARTIDA "<< "\n";
        }
        if(message.type == SPAWN_PLACE_ITEM_UPDATE){

            int pos_spaw_id = message.spawn_place_id;
            game->spawn_places[pos_spaw_id].item_id = message.item_id; 

        }

        if(message.type == ITEM_POSITION){
            //RENDERIZAR LOS ITEMS bien
            /*Item item;
            item.x = message.item_x * TILE_SIZE;
            item.y = message.item_y * TILE_SIZE;
            game.items.push_back(item);*/
        }

        if(message.type == DUCK_PICKUP_ITEM){
            int pos_id = message.player_id - 1;


            if(message.item_id == WEAPON_1_ID){
                game->ducks[pos_id].weapon_equiped = message.item_id;
            }

            if(message.item_id == HELMET_ID){
                game->ducks[pos_id].item_on_hand = message.item_id;
            }

            if(message.item_id == ARMOR_ID){
                game->ducks[pos_id].item_on_hand = message.item_id;
            }
        }

        if(message.type == DUCK_EQUIP_ITEM){
            int pos_id = message.player_id - 1;
            game->ducks[pos_id].item_on_hand = 0;


            // si es un arma la equipo como arma
            if(message.item_id == WEAPON_1_ID){
                game->ducks[pos_id].weapon_equiped = message.item_id;
            }

            // si es un helmet 

            if(message.item_id == HELMET_ID){
                game->ducks[pos_id].helmet_equiped = message.item_id;
            }

            // si es un ARMOR 

            if(message.item_id == ARMOR_ID){
                game->ducks[pos_id].armor_equiped = message.item_id;
            }
        }

        if(message.type == DROP_WEAPON){
            int pos_id = message.player_id - 1;
            game->ducks[pos_id].weapon_equiped = 0;
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
            game->ducks[pos_id].x = 400 * TILE_SIZE;
            game->ducks[pos_id].y = 400 * TILE_SIZE;
        }

        if (message.type == DUCK_POS_UPDATE){
            int pos_id = message.player_id - 1;

            game->ducks[pos_id].x = message.duck_x * TILE_SIZE;
            game->ducks[pos_id].y = message.duck_y * TILE_SIZE;

            //con looking podemos hacer que el pato mire para arriba o aletee tambien (creo)
            if(message.looking == LOOKING_LEFT){
                //pato esta mirando a la izquierda
                game->ducks[pos_id].flipType = SDL_FLIP_HORIZONTAL;

            } else {
                //pato esta mirando a la derecha
                game->ducks[pos_id].flipType = SDL_FLIP_NONE;
            }

            game->ducks[pos_id].is_moving = message.is_moving;
            game->ducks[pos_id].is_jumping = message.is_jumping;
            game->ducks[pos_id].is_fluttering = message.is_fluttering;
        }
    }
    return message;
}


//** Lobby **//
int SDLHandler::waitForStartGame() {
    int done = SUCCESS;
    bool start_game = false;

    while (!start_game && !done) {
        done = eventProcessor.processLobbyEvents(screenManager.get(), start_game);
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
    screenManager = std::make_unique<ScreenManager>(renderer, handle_textures);
    screenManager->showStartScreen();

    screenManager->loadLobbyScreen();
    screenManager->showLobbyScreen();
    if(waitForStartGame() == ERROR) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return;
    }

    GameState game{};
    game.renderer = renderer;
    game.client_game_map.setMap(map);
    game.command_queue = &command_queue;
    std::cout << "ID: " << id << "\n";

    //recibo la posicion de los N spawn places



    loadGame(&game);

    std::cout << "HORA DE RECIBIR SPAWNS" << "\n";
    for (int i = 0; i < N_SPAWN_PLACES; i++){
        // lo hago bloqueante asi no avanza hasta recibir los spawnplaces

        Message message = message_queue.pop();

        if(message.type == SPAWN_PLACE_POSITION){
            game.spawn_places.emplace_back(message.spaw_place_x * TILE_SIZE, message.spaw_place_y * TILE_SIZE, message.item_id);
        }
    }

    rendererManager->doRenderStatic(&game);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    try{
        while (!done) {
            //PRIMERO MANDO AL SERVER
            done = eventProcessor.processGameEvents(window, &game, id);

            //LUEGO RECIBO DEL SERVER Y HAGO EL RENDER
            Message message = handleMessages(&game, message_queue);
            
            if(message.type == END_ROUND){
                std::cout << "TERMINO LA RONDA"<< "\n";
                std::cout << "El ganador fue el pato "<< message.duck_winner << "\n";
                //TODO:renderizar las cosas estaticas otra vez
            }

            if(message.type == END_GAME){
                std::cout << "TERMINO LA PARTIDA"<< "\n";
                std::cout << "El ganador fue el pato "<< message.duck_winner << "\n";
                //TODO:  mostrar pantalla de victoria antes de salir
                done = ERROR;
                break;
            }

            rendererManager->doRenderDynamic(&game, message);
            //renderItems(renderer, &game);

            SDL_Delay(DELAY_TIME);
        }
    } catch (const ClosedQueue& e){
        done = ERROR;
        std::cout << "SE CERRO LA QUEUE"<< "\n";
    } catch (const LibError& l){
        done = ERROR;
        std::cout << "SE CERRO EL PROTOCOLO"<< "\n";
    }

    // Termino el juego => libero recursos
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}