#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <common/message.h>


#define DELAY_TIME 60

//using namespace SDL2pp;

SDLHandler::SDLHandler(): handle_textures(nullptr) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
}

SDLHandler::~SDLHandler() {
    SDL_Quit();
}

//** Juego **//
void SDLHandler::loadGame(GameState &game, Queue<Message> &message_queue) {
    std::vector<TextureInfo> textures_to_load = {
        {"forest", "backgrounds/forest", 1},
        {"crate", "crate", 1},
        {"gun", "guns/AK-47", 1},
        {"cowboy-pistol", "guns/cowboy_pistol", 1},
        {"laser-rifle", "guns/laser_rifle", 1},
        {"magnum", "guns/magnum", 1},
        {"pew-pew-laser", "guns/pew_pew_laser", 1},
        {"shotgun", "guns/shotgun", 1},
        {"AK-47", "guns/AK-47", 1},
        {"sniper", "guns/sniper", 1},
        {"duel-pistol", "guns/duel_pistol", 1},
        {"spawn", "spawn-place", 1},
        {"helmet", "armor/helmet", 1},
        {"armor", "armor/armor", 1},
        {"bullet", "ammo/bullet", 1},
        {"laser", "ammo/laser_flare", 2},
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

    // Recibo e inicializo los elementos del juego
    gameInitializer.initializeGame(message_queue, game, frame_width, frame_height);
    
    // Inicializo el render manager
    rendererManager = std::make_unique<RendererManager>(game.renderer, handle_textures);

    audioManager = std::make_unique<AudioManager>();

}


Message SDLHandler::handleMessages(GameState *game, Queue<Message> &message_queue) {
    Message message;
    while (message_queue.try_pop(message)) {

        if(message.type == END_ROUND){
            std::cout << "SE TERMINO LA RONDA "<< "\n";

            gameInitializer.initialize_new_round(*game, message_queue);
        }

        if(message.type == END_GAME){
            std::cout << "SE TERMINO LA PARTIDA "<< "\n";
        }

        if(message.type == SPAWN_PLACE_ITEM_UPDATE){

            int pos_spawn_id = message.spawn_place_id;
            game->spawn_places[pos_spawn_id].item_id = message.item_id;

        }

        if(message.type == SHOOT){
            //si recibo esto es que efectivamente disparé y tengo que reproducir el sonido
            const std::string path = std::string(AUDIO_PATH) + "shoot.wav";
            audioManager->loadSoundEffect(path);
            audioManager->playSoundEffect();
            audioManager->setSoundEffectVolume(70);

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

            // Acá reemplazar con los ids de las otras armas cuando tengas los renders de las mismas
            if((message.item_id == BASE_WEAPON_ID)|| (message.item_id == GRANADA_ID) || (message.item_id == BANANA_ID)){
                //game->ducks[pos_id].weapon_equiped = message.item_id;
                game->ducks[pos_id].weapon_equiped = BASE_WEAPON_ID;
            }

            if((message.item_id == PEW_PEW_LASER_ID) || (message.item_id == LASER_RIFLE_ID) || (message.item_id == AK_47_ID) || 
            (message.item_id == COWBOY_PISTOL_ID) || (message.item_id == MAGNUM_ID) || (message.item_id == SHOTGUN_ID) ||
            (message.item_id == DUEL_PISTOL_ID) || (message.item_id == SNIPER_ID)){
                
                std::cout << "agarre un ARMA A: " << static_cast<int>(message.item_id) << "\n";
                game->ducks[pos_id].weapon_equiped = message.item_id;
            }            

            if(message.item_id == HELMET_ID){
                std::cout << "agarre un casco" << "\n";

                game->ducks[pos_id].item_on_hand = message.item_id;
            }

            if(message.item_id == ARMOR_ID){
                std::cout << "agarre un armor" << "\n";

                game->ducks[pos_id].item_on_hand = message.item_id;
            }
        }

        if(message.type == DUCK_EQUIP_ITEM){
            int pos_id = message.player_id - 1;
            game->ducks[pos_id].item_on_hand = 0;


            // si es un arma la equipo como arma
            // Acá reemplazar con los ids de las otras armas cuando tengas los renders de las mismas
            if((message.item_id == BASE_WEAPON_ID)|| (message.item_id == GRANADA_ID) || (message.item_id == BANANA_ID)){
                //game->ducks[pos_id].weapon_equiped = message.item_id;
                game->ducks[pos_id].weapon_equiped = BASE_WEAPON_ID;
            }

            if((message.item_id == PEW_PEW_LASER_ID) || (message.item_id == LASER_RIFLE_ID) || (message.item_id == AK_47_ID) || 
            (message.item_id == COWBOY_PISTOL_ID) || (message.item_id == MAGNUM_ID) || (message.item_id == SHOTGUN_ID) ||
            (message.item_id == DUEL_PISTOL_ID) || (message.item_id == SNIPER_ID)){
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
            int pos_id = message.player_id - 1;
            game->ducks[pos_id].armor_equiped = 0;
        }

        if(message.type == HELMET_BROKEN){
            int pos_id = message.player_id - 1;
            game->ducks[pos_id].helmet_equiped = 0; 
        }

        if(message.type == BULLET_POS_UPDATE){
            std::cout << "Sdl Handler, id" << message.bullet_id << " y type " << message.bullet_type << ", en x: " << 
            message.bullet_x << " y: " << message.bullet_y << std::endl;
            bool includes = false;
            for (Projectile& projectile: game->projectiles) {
                if (projectile.id == message.bullet_id) {
                    includes = true;
                    projectile.old_x = projectile.current_x;
                    projectile.old_y = projectile.current_y;
                    projectile.current_x = message.bullet_x;
                    projectile.current_y = message.bullet_y;
                    std::cout << "Ya tenia ese id, lo actualizo \n";
                }
            }
            if (!includes) {
                game->projectiles.push_back(Projectile{message.bullet_x, message.bullet_y, 10000, 10000, message.bullet_id, message.bullet_type, 0});
                std::cout << "No tenia ese id, lo meto \n";
            }
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
    int id_match = 1;

    while (!start_game && !done) {
        done = eventProcessor.processLobbyEvents(screenManager.get(), start_game, id_match);
    }
    return done;
}

void SDLHandler::run(Queue<Command>& command_queue, uint16_t id, Queue<Message>& message_queue) {
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
    screenManager->renderStaticLobby();
    screenManager->showLobbyScreen();
    if(waitForStartGame() == ERROR) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        return;
    }


    GameState game{};
    game.renderer = renderer;
    game.command_queue = &command_queue;
    std::cout << "ID: " << id << "\n";

    loadGame(game, message_queue);


    //Empiezo la musica de fondo
    const std::string path = std::string(AUDIO_PATH) +"ambient-music.wav";
    audioManager->loadMusic(path);
    audioManager->playMusic(-1); //musica en bucle infinitamente
    audioManager->setMusicVolume(30);

    //Renderizo lo estatico
    rendererManager->doRenderStatic(&game);

    // Event Loop: La ventana se abre => se entra al loop
    int done = SUCCESS;
    try{
        while (!done) {
            //PRIMERO MANDO AL SERVER
            
            //no se si pasar el audio manager aca para reproducir el sonido del disparo es lo mejor 
            //pero por ahora funciona... 
            done = eventProcessor.processGameEvents(window, &game, id);



            //LUEGO RECIBO DEL SERVER Y HAGO EL RENDER
            Message message = handleMessages(&game, message_queue);
            
            if(message.type == END_ROUND){
                std::cout << "TERMINO LA RONDA"<< "\n";
                std::cout << "El ganador fue el pato "<< static_cast<char>(message.duck_winner) << "\n";

                rendererManager->doRenderStatic(&game);
                continue;
                
            }

            if(message.type == END_GAME){
                std::cout << "TERMINO LA PARTIDA"<< "\n";
                std::cout << "El ganador fue el pato "<< static_cast<char>(message.duck_winner)  << "\n";
                //TODO:  mostrar pantalla de victoria antes de salir
                done = ERROR;
                break;
            }

            rendererManager->doRenderDynamic(&game, message);

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
    Mix_CloseAudio();
}