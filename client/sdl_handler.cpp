#include "sdl_handler.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <common/message.h>
#include <map>

//using namespace SDL2pp;

SDLHandler::SDLHandler(): is_alive(true), handle_textures(nullptr), duck_id(0), lobby_exit(false) {
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
        {"camp-fire", "backgrounds/camp-fire", 1},
        {"city", "backgrounds/city", 1},
        {"industrial", "backgrounds/industrial", 1},
        {"miami", "backgrounds/miami", 1},
        {"moon", "backgrounds/moon-mountain", 1},
        {"spring", "backgrounds/spring", 1},
        {"crate", "crate", 1},
        {"lava", "platforms/lava", 1},
        {"musgo", "platforms/musgo", 1},
        {"nieve", "platforms/nieve", 1},
        {"pasto", "platforms/pasto", 1},
        {"pasto-tierra", "platforms/pasto-tierra", 1},
        {"piedra", "platforms/piedra", 1},
        {"piedrapasto", "platforms/piedrapasto", 1},
        {"tierra", "platforms/tierra", 1},
        {"box", "box", 1},
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
        {"grenade-pin", "grenades/grenade_with_pin", 1},
        {"grenade", "grenades/grenade_empty", 1},
        {"banana", "bananas/banana", 1},
        {"banana-floor", "bananas/banana_floor", 1},
        {"duck-walking", "duck-walking", 6},
        {"duck-walking-wings", "duck-walking-wings", 1},
        {"duck-jumping", "duck-jumping", 1},
        {"duck-jumping-wings", "duck-jumping-wings", 1},
        {"duck-fluttering", "duck-fluttering", 6},
        {"duck", "duck", 1},
        {"corazon", "corazon", 1},
        {"duck-laying-down", "duck-laying-down", 1},
        {"grave", "grave", 1}
    };

    // Cargo las texturas
    int frame_width = 0, frame_height = 0;
    for (const TextureInfo& texture_info : textures_to_load) {
        handle_textures.loadTexture(texture_info, &frame_width, &frame_height);
    }

    // Recibo e inicializo los elementos del juego
    gameInitializer.initializeGame(message_queue, game, frame_width, frame_height);
    
    // Inicializo el render manager
    camera = std::make_unique<Camera>();
    rendererManager = std::make_unique<RendererManager>(game.renderer, handle_textures, *camera);

    audioManager = std::make_unique<AudioManager>();

}


Message SDLHandler::handleMessages(GameState *game, Queue<Message> &message_queue) {

    std::map<uint8_t, int> weaponAmmo = {
            {PEW_PEW_LASER_ID, 12},
            {LASER_RIFLE_ID, 10},
            {AK_47_ID, 30},
            {DUEL_PISTOL_ID, 1},
            {COWBOY_PISTOL_ID, 6},
            {MAGNUM_ID, 6},
            {SHOTGUN_ID, 2},
            {SNIPER_ID, 3}
        };

    Message message;
    while (message_queue.try_pop(message)) {

        if(message.type == END_GAME){
            std::cout << "SE TERMINO LA PARTIDA "<< "\n";

            screenManager->showScoreboard(message.scoreboard, true);

            screenManager->showEndMatchScreen(message.duck_winner);
            message.type = END_GAME;
        }

        if(message.type == END_ROUND){
            std::cout << "SE TERMINO LA RONDA "<< "\n";

            gameInitializer.initialize_new_round(*game, message_queue);
            std::cout << "SE INICIALIZO EL GAME "<< "\n";
            screenManager->showNextRoundScreen(message.duck_winner);
            screenManager->showGetReadyScreen(message.round);
            std::cout << "refresco lo estatico "<< "\n";
            rendererManager->doRenderStatic(game);

        }

        if(message.type == END_FIVE_ROUNDS){
            std::cout << "PASARON 5 RONDAS "<< "\n";

            gameInitializer.initialize_new_round(*game, message_queue);
            std::cout << "Se inicializo el game "<< "\n";
            screenManager->showNextRoundScreen(message.duck_winner);

            screenManager->showScoreboard(message.scoreboard, false);

            screenManager->showGetReadyScreen(message.round);

            std::cout << "refresco lo estatico "<< "\n";
            rendererManager->doRenderStatic(game);

        }

        if(message.type == SPAWN_PLACE_ITEM_UPDATE){

            int pos_spawn_id = message.spawn_place_id;
            game->spawn_places[pos_spawn_id].item_id = message.item_id;

        }

        if(message.type == SHOOT){

            int pos_id = message.player_id - 1;
            if(game->ducks[pos_id].current_ammo > 0){
                game->ducks[pos_id].current_ammo -= 1;
            }
            //si recibo esto es que efectivamente disparé y tengo que reproducir el sonido
            const std::string path = std::string(AUDIO_PATH) + "shoot.wav";
            audioManager->loadSoundEffect(path);
            audioManager->playSoundEffect();
            audioManager->setSoundEffectVolume(100);
        }

        if(message.type == BOX_DESTROYED){
            std::cout << "Se destruyo la caja con id: " << message.box_id << "\n";

            int box_id = message.box_id;
            game->boxes[box_id].destroyed = true;
            game->boxes[box_id].item_id = message.item_id;

        }

        if (message.type == ITEM_ON_FLOOR_UPDATE) {
            int x = message.item_x * TILE_SIZE;
            int y = message.item_y * TILE_SIZE;

            std::cout << "AGARRE EL ITEM QUE ESTA EN X: "<< x << " Y: "<< y << "\n";


            // Recorrer la lista de items en el suelo hasta encontrar el que matchea con la pos que me llegó
            for (auto& item_on_floor : game->items_on_floor) {
                std::cout << "RECORRO LA LISTA DE ITEMS EN EL SUELO X: "<< item_on_floor.x << " Y: "<< item_on_floor.y << "\n";
                
                if (item_on_floor.x == x && item_on_floor.y == y) {
                    std::cout << "Item found at (" << x << ", " << y << ") with ID: " << static_cast<int>(item_on_floor.item_id) << std::endl;

                    item_on_floor.item_id = 0;
                }
            }
        }


         if(message.type == THROWABLE_ITEM) {
            std::cout << "Llega un throwable item de pos x: " << message.item_x << " y: " << 
            message.item_y << " y un id " << message.item_id << ", lo meto a la lista \n";

            bool includes = false;
            for (ThrowedItem& throwed: game->throwed_items) {
                if (throwed.current_x == message.item_x && throwed.current_y == message.item_y) {
                    includes = true;
                    throwed.current_x = message.item_x;
                    throwed.current_y = message.item_y;
                    throwed.type = message.item_id;
                    throwed.touching_floor = message.item_touching_floor;
                    throwed.used = message.item_used;
                    std::cout << "Ya tenia esa pos, lo actualizo \n";
                }
            }
            if (!includes) {
                ThrowedItem item = ThrowedItem { message.item_x * TILE_SIZE, message.item_y * TILE_SIZE, message.item_id, message.item_used, message.item_touching_floor};
                game->throwed_items.push_back(item);
                std::cout << "No tenia esa pos, lo meto \n";
            }
        } 


        if(message.type == DUCK_PICKUP_ITEM){
            int pos_id = message.player_id - 1;

            if((message.item_id == PEW_PEW_LASER_ID) || (message.item_id == LASER_RIFLE_ID) || (message.item_id == AK_47_ID) || 
            (message.item_id == COWBOY_PISTOL_ID) || (message.item_id == MAGNUM_ID) || (message.item_id == SHOTGUN_ID) ||
            (message.item_id == DUEL_PISTOL_ID) || (message.item_id == SNIPER_ID)|| (message.item_id == GRENADE_ID)
             || (message.item_id == BANANA_ID) || (message.item_id == BASE_WEAPON_ID)){
                std::cout << "agarre un ARMA A: " << static_cast<int>(message.item_id) << "\n";
                game->ducks[pos_id].weapon_equiped = message.item_id;
                game->ducks[pos_id].current_ammo = weaponAmmo[message.item_id];

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

            if((message.item_id == PEW_PEW_LASER_ID) || (message.item_id == LASER_RIFLE_ID) || (message.item_id == AK_47_ID) || 
            (message.item_id == COWBOY_PISTOL_ID) || (message.item_id == MAGNUM_ID) || (message.item_id == SHOTGUN_ID) ||
            (message.item_id == DUEL_PISTOL_ID) || (message.item_id == SNIPER_ID)|| (message.item_id == GRENADE_ID)
             || (message.item_id == BANANA_ID) || (message.item_id == BASE_WEAPON_ID)){
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
            game->ducks[pos_id].current_ammo = 0;
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
                    projectile.horizontal = message.bullet_horizontal;
                    std::cout << "Ya tenia ese id, lo actualizo \n";
                }
            }
            if (!includes) {
                game->projectiles.push_back(Projectile{message.bullet_x, message.bullet_y, 10000, 10000, message.bullet_id, message.bullet_type, 0, message.bullet_horizontal});
                std::cout << "No tenia ese id, lo meto \n";
            }
        }

        if(message.type == KILL_DUCK){
            int pos_id = message.player_id - 1;
            //NO LO ELIMINO DE LA LISTA PORQUE ALTO KILOMBO ASI QUE MUEVO EL DIBUJO AFUERA DE LA PANTALLA
            //game->ducks[pos_id].x = 400 * TILE_SIZE;
            //game->ducks[pos_id].y = 400 * TILE_SIZE;
            game->ducks[pos_id].is_dead = true;
        }

        if (message.type == DUCK_POS_UPDATE){

            // std::cout << "RECIBO POS UPDATE" << "\n";

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
            game->ducks[pos_id].is_laying_down = message.is_laying_down;
            game->ducks[pos_id].is_looking_up = message.is_looking_up;

        }
    }
    return message;
}


//** Lobby **//
int SDLHandler::waitForStartGame(uint16_t lobby_id, Queue<Command>& command_queue, Queue<Message>& message_queue, ClientProtocol& protocol) {
    int done = SUCCESS;
    int chosen_match = 0;
    bool selected_match = false;
    std::cout << "Seleccionando... " << selected_match << "\n";

    while (is_alive) {
        const auto start = std::chrono::high_resolution_clock::now();

        done = eventProcessor.processLobbyEvents(screenManager.get(), command_queue, lobby_id, is_alive, chosen_match, selected_match);

        try {
            Message message;
            if (message_queue.try_pop(message)) {
                if (message.type == EXIT_GAME){
                    std::cout << "Lo siento, parece que el servidor cerro" << "\n";
                    lobby_exit = true;
                    is_alive = false;
                    done = ERROR;
                    screenManager->showServerIsDownScreen();
                    break;
                }

                if (message.type == NEW_MATCH_CODE){
                    std::cout << "Partida creada con id: " << static_cast<int>(message.current_match_id) << "\n";
                    screenManager->renderNewMatchText(message.current_match_id);
                }

                if (message.type == LIST_MATCH_AVAILABLE) {
                    screenManager->renderAvailableMatches(message.existing_matches);
                }

                if (message.type == EXISTING_MATCH_CODE){
                    std::cout << "Conectado a partida con id: " << static_cast<int>(message.current_match_id) << "\n";
                }

                if (message.type == LOBBY_COMMAND_FAIL){
                    std::cout << "Ups! parece que no puedes realizar esa accion" << "\n";
                }

                if (message.type == START_MATCH_CODE){
                    chosen_match = message.current_match_id;
                    std::cout << "Partida iniciada con id: " << static_cast<int>(chosen_match) << "\n";
                    if (protocol.send_command(Command(lobby_id, LOBBY_STOP_CODE))){
                        std::cout << "Me desconecte del lobby. Ahora voy a comunicarme con el juego" << "\n";
                        is_alive = false;
                        break;
                    }
                }

                std::cout << "\n";
            }

        } catch (const ClosedQueue& e) {
            done = ERROR;
            is_alive = false;
            std::cerr << "Exception handling the lobby: ClosedQueue" << e.what() << std::endl;

        } catch (const LibError& e) {
            done = ERROR;
            is_alive = false;
            std::cerr << "Exception handling the lobby: LibError" << e.what() << std::endl;

        } catch (const std::exception& e) {
            done = ERROR;
            is_alive = false;
            std::cerr << "Exception handling the lobby: " << e.what() << std::endl;
        }

        const auto end = std::chrono::high_resolution_clock::now();
        auto loop_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        auto sleep_duration = DELAY_TIME - loop_duration;

        if (sleep_duration > 0) {
            SDL_Delay(sleep_duration);
        }
    }
    return done;
}

void SDLHandler::initializeWindow(SDL_Window*& window, SDL_Renderer*& renderer) {
    window = SDL_CreateWindow("Duck Game",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    handle_textures = TextureHandler(renderer);
    screenManager = std::make_unique<ScreenManager>(renderer, handle_textures);
    screenManager->showStartScreen();
}

int SDLHandler::runGame(SDL_Window *window, SDL_Renderer *renderer, Queue<Command> &command_queue, Queue<Message> &message_queue) {
    GameState game{};
    game.renderer = renderer;
    game.command_queue = &command_queue;
    game.music = true;
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
            const auto start = std::chrono::high_resolution_clock::now();

            done = eventProcessor.processGameEvents(window, &game, duck_id);

            if(game.music){
                audioManager->setMusicVolume(30);
                audioManager->setSoundEffectVolume(100);
            }

            if(!game.music){
                audioManager->setMusicVolume(0);
                audioManager->setSoundEffectVolume(0);
            }

            Message message = handleMessages(&game, message_queue);

            if(message.type == END_GAME){
                std::cout << "TERMINO LA PARTIDA"<< "\n";
                std::cout << "El ganador fue el pato "<< message.duck_winner  << "\n";
                done = ERROR;
                break;
            }

            if(message.type == EXIT_GAME){
                std::cout << "Lo siento, parece que el servidor cerró"<< "\n";
                screenManager->showServerIsDownScreen();
                done = ERROR;
                break;
            }

            rendererManager->doRenderDynamic(&game, message, duck_id);

            const auto end = std::chrono::high_resolution_clock::now();
            auto loop_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            auto sleep_duration = DELAY_TIME - loop_duration;

            if (sleep_duration > 0) {
                SDL_Delay(sleep_duration);
            }
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
    return done;
}

int SDLHandler::run(uint16_t lobby_id, Queue<Command>& command_queue, Queue<Message>& message_queue, ClientProtocol& protocol) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    initializeWindow(window, renderer);

    //manejo la interaccion con el lobby
    screenManager->loadLobbyScreen();
    screenManager->renderStaticLobby();
    screenManager->showLobbyScreen();
    if(waitForStartGame(lobby_id, command_queue, message_queue, protocol) == ERROR) {
        std::cout << "Parece que hubo un error, el juego no empezara"<< std::endl;
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        command_queue.close();
        message_queue.close();
        return ERROR;
    }


    //recibo el nuevo id, el cual corresponde a mi pato
    Message first_game_message = message_queue.pop();
    duck_id = first_game_message.player_id;
    std::cout << "My DUCK ID is: " << duck_id  << std::endl;

    int result = runGame(window, renderer, command_queue, message_queue);
    command_queue.close();
    message_queue.close();

    return result;
}
