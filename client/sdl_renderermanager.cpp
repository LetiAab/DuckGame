#include "sdl_renderermanager.h"

RendererManager::RendererManager(SDL_Renderer* renderer, TextureHandler& texture_handler):
    renderer(renderer), texture_handler(texture_handler) {}

//** Estatico **//
void RendererManager::doRenderStatic(GameState* game) {
    SDL_Texture* static_scene = texture_handler.createRenderTarget("static_scene", WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderTarget(renderer, static_scene);

    // Renderizo los objetos estaticos
    SDL_RenderCopy(renderer, texture_handler.getTexture("forest"), NULL, NULL);

    for (auto & crate : game->crates) {
        SDL_Rect crate_rect = {crate.x, crate.y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, texture_handler.getTexture("crate"), NULL, &crate_rect);
    }

    //RENDERIZO LOS SPAWN PLACES.    
    for (auto & spawn_place : game->spawn_places) {

        SDL_Rect spawn_rect = {spawn_place.x, spawn_place.y, TILE_SIZE * 6, TILE_SIZE * 4};
        SDL_RenderCopy(renderer, texture_handler.getTexture("spawn"), NULL, &spawn_rect);

  }

    SDL_SetRenderTarget(renderer, NULL);
}


//** Dinamico **//
void RendererManager::renderBullet(GameState* game, const int size) {
    for (Projectile& projectile: game->projectiles) {    
        if (projectile.type == 0) {
            SDL_Rect bulletRect = { projectile.current_x * TILE_SIZE, projectile.current_y * TILE_SIZE, size, size };
            SDL_RenderCopy(renderer, texture_handler.getTexture("bullet"), NULL, &bulletRect);
            projectile.times_repeated++;
        } else {
            SDL_Rect laserRect = { projectile.current_x * TILE_SIZE, projectile.current_y * TILE_SIZE, size, size };
            SDL_RenderCopy(renderer, texture_handler.getTexture("laser"), NULL, &laserRect);        
            projectile.times_repeated++;
        }
    }

    // Saco las balas y laseres que repitieron posición en el mapa
    for (auto it = game->projectiles.begin(); it != game->projectiles.end();) {
        if (it->times_repeated > 1) {
            it = game->projectiles.erase(it);
        } else {
            ++it; // Solo avanzas si no eliminaste
        }
    }
}

void RendererManager::renderDucks(GameState* game) {
    for (int i = 0; i < game->ducks_quantity; i++) {
        Duck& duck = game->ducks[i];

        // Cambiar el fotograma de animación si el pato está en movimiento
        if ((duck.is_moving && !duck.is_jumping) || duck.is_fluttering) {
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

        SDL_Texture* duck_texture = nullptr;
        SDL_Texture* wings_texture = nullptr;
        if (duck.is_jumping && !duck.is_fluttering) {
            duck_texture = texture_handler.getTexture("duck-jumping");
            wings_texture = texture_handler.getTexture("duck-jumping-wings");
        } else if (duck.is_fluttering) {
            duck_texture = texture_handler.getTexture("duck-fluttering");
            //wings_texture = texture_handler.getTexture("duck-fluttering-wings");
        } else {
            duck_texture = texture_handler.getTexture("duck-walking");
            wings_texture = texture_handler.getTexture("duck-walking-wings");
        }

        SDL_SetTextureColorMod(duck_texture, colors[i][0], colors[i][1], colors[i][2]);
        SDL_RenderCopyEx(renderer, duck_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
        SDL_SetTextureColorMod(duck_texture, 255, 255, 255); //reseteo el color

        if(duck.weapon_equiped != 0){
            //por ahora siempre renderiza el mismo arma. Hay que valdiar contra los diferentes ids de armas que no existen aun;
            if (duck.weapon_equiped == COWBOY_PISTOL_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("cowboy-pistol"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == LASER_RIFLE_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("laser-rifle"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == MAGNUM_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("magnum"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == PEW_PEW_LASER_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("pew-pew-laser"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == SHOTGUN_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("shotgun"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == AK_47_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("AK-47"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == SNIPER_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("sniper"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == DUEL_PISTOL_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("duel-pistol"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("gun"), NULL, &gun_rect, 0, NULL, duck.flipType);
            }
        }

        if(duck.item_on_hand != 0){

            if (duck.item_on_hand == HELMET_ID){
                SDL_Rect helmet_rect = {
                    duck.x + TILE_SIZE * 6,
                    (duck.y + TILE_SIZE * 2),
                    (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                    (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
                      };

                SDL_RenderCopyEx(renderer, texture_handler.getTexture("helmet"), NULL, &helmet_rect, 0, NULL, duck.flipType);
            }
            
            if (duck.item_on_hand == ARMOR_ID){
                SDL_Rect armor_rect = {
                    duck.x + TILE_SIZE * 6 + TILE_SIZE * 3,
                    (duck.y + TILE_SIZE * 2),
                    (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                    (TILE_SIZE * DUCK_SIZE_Y / 2)
                      };

                SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &armor_rect, 0, NULL, duck.flipType);
            }



            

        }

        if(duck.helmet_equiped != 0){
            
            SDL_Rect helmet_rect = {
                duck.x + TILE_SIZE * 2,
                (duck.y - TILE_SIZE/4) - TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };

            SDL_RenderCopyEx(renderer, texture_handler.getTexture("helmet"), NULL, &helmet_rect, 0, NULL, duck.flipType);

        }
        if(duck.armor_equiped != 0){

            SDL_Rect armor_rect = {
                duck.x + TILE_SIZE * 2,
                (duck.y  + TILE_SIZE * 6),
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2)
            };

            SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &armor_rect, 0, NULL, duck.flipType);
        }


        if (wings_texture != nullptr) {
            SDL_SetTextureColorMod(wings_texture, colors[i][0], colors[i][1], colors[i][2]);
            SDL_RenderCopyEx(renderer, wings_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
            SDL_SetTextureColorMod(wings_texture, 255, 255, 255); //reseteo el color
        }

    }
}

void RendererManager::renderBoxes(GameState* game){
        for (auto& box : game->boxes) {
    
            if(!box.destroyed){


                SDL_Rect box_rect = { 
                    box.x, 
                    box.y, 
                    TILE_SIZE * BOX_SIZE_X, 
                    TILE_SIZE * BOX_SIZE_Y 
                };
            
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("box"), NULL, &box_rect, 0, NULL, SDL_FLIP_NONE);

            } else if (!box.item_taked) {
                renderItem(box.item_id, box.x, box.y + 20);
            }

        }


}
void RendererManager::renderItem(uint8_t item_id, int x, int y){
     SDL_Rect rect;

    if (item_id == BASE_WEAPON_ID || item_id == GRANADA_ID || item_id == BANANA_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("gun"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == COWBOY_PISTOL_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("cowboy-pistol"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == LASER_RIFLE_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("laser-rifle"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == MAGNUM_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("magnum"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == PEW_PEW_LASER_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("pew-pew-laser"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == SHOTGUN_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("shotgun"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == AK_47_ID) {
        rect = { x, y - TILE_SIZE * 11, TILE_SIZE * DUCK_SIZE_X, TILE_SIZE * DUCK_SIZE_Y };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("AK-47"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == SNIPER_ID || item_id == DUEL_PISTOL_ID || item_id == HELMET_ID) {
        rect = { x, y - TILE_SIZE * 9, (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE, (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture(
            item_id == SNIPER_ID ? "sniper" :
            item_id == DUEL_PISTOL_ID ? "duel-pistol" : "helmet"
        ), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == ARMOR_ID) {
        rect = { x - TILE_SIZE, y - TILE_SIZE * 8, (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE, (TILE_SIZE * DUCK_SIZE_Y / 2) };
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    }
}


void RendererManager::renderItems(GameState* game) {


    for (auto& spawn_place : game->spawn_places) {
        // Acá reemplazar con los ids de las otras armas cuando tengas los renders de las mismas
        renderItem(spawn_place.item_id, spawn_place.x, spawn_place.y);
    }
}

void RendererManager::renderStats(GameState* game, uint16_t id) {
    if (id == 0 || id > game->ducks_quantity) {
        std::cerr << "ID fuera de rango: " << id << std::endl;
        return;
    }

    int pos_id = id - 1; 
    Duck& duck = game->ducks[pos_id];

    SDL_Rect duck_rect = {
        10, 
        10,
        TILE_SIZE * DUCK_SIZE_X,
        TILE_SIZE * DUCK_SIZE_Y 
    };

    SDL_Texture* duck_texture = texture_handler.getTexture("duck");
    SDL_SetTextureColorMod(duck_texture, colors[pos_id][0], colors[pos_id][1], colors[pos_id][2]);
    SDL_RenderCopyEx(renderer, duck_texture, NULL, &duck_rect, 0, NULL, SDL_FLIP_NONE);
    SDL_SetTextureColorMod(duck_texture, 255, 255, 255);


    SDL_Rect heart_rect = {
        60, 
        30,
        TILE_SIZE * 6,
        TILE_SIZE * 6 
    };
    SDL_Texture* heart_texture = texture_handler.getTexture("corazon");
    SDL_RenderCopyEx(renderer, heart_texture, NULL, &heart_rect, 0, NULL, SDL_FLIP_NONE);

    if (duck.armor_equiped || duck.helmet_equiped){
        SDL_Rect heart_rect = {
            85, 
            30,
            TILE_SIZE * 6,
            TILE_SIZE * 6 
        };
        SDL_Texture* heart_texture = texture_handler.getTexture("corazon");
        SDL_RenderCopyEx(renderer, heart_texture, NULL, &heart_rect, 0, NULL, SDL_FLIP_NONE);
    }

    if (duck.armor_equiped && duck.helmet_equiped){
        SDL_Rect heart_rect = {
            110, 
            30,
            TILE_SIZE * 6,
            TILE_SIZE * 6 
        };
        SDL_Texture* heart_texture = texture_handler.getTexture("corazon");
        SDL_RenderCopyEx(renderer, heart_texture, NULL, &heart_rect, 0, NULL, SDL_FLIP_NONE);
    }

}


void RendererManager::doRenderDynamic(GameState* game, Message& message, uint16_t id) {
    SDL_RenderCopy(renderer, texture_handler.getTexture("static_scene"), NULL, NULL);


    renderDucks(game);
    renderItems(game);
    renderBoxes(game);
    renderStats(game, id);

    if (message.type == BULLET_POS_UPDATE){
        renderBullet(game);
    }

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}