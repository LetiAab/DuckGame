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
            // Para renderizar el arma en la mano del pato
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
            } else if (duck.weapon_equiped == GRENADE_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("grenade-pin"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == BANANA_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("banana"), NULL, &gun_rect, 0, NULL, duck.flipType);
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

void RendererManager::renderItems(GameState* game) {


    for (auto& spawn_place : game->spawn_places) {
        // Para renderizar en el spawn_place
        if (spawn_place.item_id == BASE_WEAPON_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("gun"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == COWBOY_PISTOL_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("cowboy-pistol"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == LASER_RIFLE_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("laser-rifle"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == MAGNUM_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("magnum"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == PEW_PEW_LASER_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("pew-pew-laser"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == SHOTGUN_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("shotgun"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == AK_47_ID) {
            SDL_Rect gun_rect = { 
                spawn_place.x, 
                spawn_place.y - TILE_SIZE * 11, 
                TILE_SIZE * DUCK_SIZE_X, 
                TILE_SIZE * DUCK_SIZE_Y 
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("AK-47"), NULL, &gun_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == SNIPER_ID) {
            SDL_Rect helmet_rect = {
                spawn_place.x,
                spawn_place.y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("sniper"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == DUEL_PISTOL_ID) {
            SDL_Rect helmet_rect = {
                spawn_place.x,
                spawn_place.y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("duel-pistol"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == GRENADE_ID) {
            SDL_Rect helmet_rect = {
                spawn_place.x,
                spawn_place.y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("grenade-pin"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == BANANA_ID) {
            SDL_Rect helmet_rect = {
                spawn_place.x,
                spawn_place.y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("banana"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == HELMET_ID) {
            SDL_Rect helmet_rect = {
                spawn_place.x,
                spawn_place.y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("helmet"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        } else if (spawn_place.item_id == ARMOR_ID) {
            SDL_Rect armor_rect = {
                spawn_place.x - TILE_SIZE,
                spawn_place.y - TILE_SIZE * 8,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2)
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &armor_rect, 0, NULL, SDL_FLIP_NONE);
        }
    }
}



void RendererManager::renderThrowed(GameState* game) {
    for (auto& throwed_item : game->throwed_items) {
        //if (throwed_item.used)
        //    continue;
        // Para renderizar en el spawn_place
        if (throwed_item.type == GRENADE_ID) {
            SDL_Rect helmet_rect = {
                throwed_item.current_x,
                throwed_item.current_y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("grenade"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        }
        if (throwed_item.type == BANANA_ID) {
            std::cout << "La banana pos x: " << throwed_item.current_x << " y: " << 
            throwed_item.current_y << " y un id " << throwed_item.type << " used: " << 
            (int)throwed_item.used << " y touching_floor" << (int)throwed_item.touching_floor << ", lo meto a la lista \n";
            if (throwed_item.used) {
                std::cout << "Throwed item es true \n";
            }
            SDL_Rect helmet_rect = {
                throwed_item.current_x,
                throwed_item.current_y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE
            };
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("banana-floor"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        }
    }
    for (auto it = game->throwed_items.begin(); it != game->throwed_items.end();) {
        if (it->used || !it->touching_floor) {
            std::cout << "Elimino un elemento" << "\n";
            it = game->throwed_items.erase(it);
        } else {
            ++it; // Solo avanzas si no eliminaste
        }
    }
}

void RendererManager::doRenderDynamic(GameState* game, Message& message) {
    SDL_RenderCopy(renderer, texture_handler.getTexture("static_scene"), NULL, NULL);


    renderDucks(game);
    renderItems(game);

    if (message.type == BULLET_POS_UPDATE){
        renderBullet(game);
    }

    renderThrowed(game);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}