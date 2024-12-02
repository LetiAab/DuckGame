#include "sdl_renderermanager.h"

static float float_time = 0.0f;

RendererManager::RendererManager(SDL_Renderer* renderer, TextureHandler& texture_handler, Camera& camera):
    renderer(renderer), texture_handler(texture_handler), camera(camera), background_index(0),
    backgrounds({"forest", "camp-fire", "city", "miami", "industrial", "moon", "spring", }),
    platforms({"crate", "lava", "nieve", "pasto", "pasto-tierra", "musgo", "piedra", "piedrapasto", "tierra"}){}

//** Estatico **//
void RendererManager::doRenderStatic(GameState* game) {
    static_scene = texture_handler.createRenderTarget("static_scene", WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderTarget(renderer, static_scene);

    // Renderizo los objetos estaticos
    const std::string& background = backgrounds[background_index];
    SDL_RenderCopy(renderer, texture_handler.getTexture(background), NULL, NULL);
    background_index = (background_index + 1) % backgrounds.size();

    const std::string& platform = platforms[platform_index];
    for (auto & crate : game->crates) {
        SDL_Rect crate_rect = {crate.x, crate.y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, texture_handler.getTexture(platform), NULL, &crate_rect);
    }
    platform_index = (platform_index + 1) % platforms.size();

    // RENDERIZO LOS SPAWN PLACES
    for (auto & spawn_place : game->spawn_places) {
        SDL_Rect spawn_rect = {spawn_place.x, spawn_place.y, TILE_SIZE * 6, TILE_SIZE * 4};
        SDL_RenderCopy(renderer, texture_handler.getTexture("spawn"), NULL, &spawn_rect);
  }

    SDL_SetRenderTarget(renderer, NULL);
}

//** Dinamico **//
#include <iostream> // Para std::cout

SDL_Rect RendererManager::transformToCameraSpace(float obj_x, float obj_y, float obj_w, float obj_h) {
    return SDL_Rect{
        static_cast<int>((obj_x - camera.getX()) * camera.getZoom()),
        static_cast<int>((obj_y - camera.getY()) * camera.getZoom()),
        static_cast<int>(obj_w * camera.getZoom()),
        static_cast<int>(obj_h * camera.getZoom())
    };
}

void RendererManager::renderBullet(GameState* game, const int size) {
    for (Projectile& projectile : game->projectiles) {    

        SDL_Rect bulletRect = transformToCameraSpace(
            projectile.current_x * TILE_SIZE,
            projectile.current_y * TILE_SIZE,
            size,
            size);

        if (projectile.type == 0) {
            double angle = projectile.horizontal ? 0 : -90; 
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("bullet"), NULL, &bulletRect, angle, NULL, SDL_FLIP_NONE);
            projectile.times_repeated++;
        } else {
            double angle = projectile.horizontal ? 0 : -90; 
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("laser"), NULL, &bulletRect, angle, NULL, SDL_FLIP_NONE);        
            projectile.times_repeated++;
        }
    }

    // Saco las balas y láseres que repitieron posición en el mapa
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

        if(duck.is_dead){
            duck.is_laying_down = true;
        }

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

        SDL_Rect duck_rect = transformToCameraSpace(
                static_cast<float>(duck.x),
                static_cast<float>(duck.y),
                TILE_SIZE * DUCK_SIZE_X,
                TILE_SIZE * DUCK_SIZE_Y);

        SDL_Rect gun_rect = transformToCameraSpace(
                static_cast<float>(duck.x),
                static_cast<float>(duck.y) + TILE_SIZE / 4.0f,
                TILE_SIZE * DUCK_SIZE_X,
                TILE_SIZE * DUCK_SIZE_Y);

        SDL_Texture* duck_texture = nullptr;
        SDL_Texture* wings_texture = nullptr;
        if (duck.is_laying_down) {
            duck_texture = texture_handler.getTexture("duck-laying-down");
        } else if (duck.is_jumping && !duck.is_fluttering) {
            duck_texture = texture_handler.getTexture("duck-jumping");
            wings_texture = texture_handler.getTexture("duck-jumping-wings");
        } else if (duck.is_fluttering) {
            duck_texture = texture_handler.getTexture("duck-fluttering");
        } else if (duck.is_looking_up){
            duck_texture = texture_handler.getTexture("duck-walking");
            wings_texture = texture_handler.getTexture("duck-walking-wings");
        
        } else {
            duck_texture = texture_handler.getTexture("duck-walking");
            wings_texture = texture_handler.getTexture("duck-walking-wings");
        }

        SDL_SetTextureColorMod(duck_texture, colors[i][0], colors[i][1], colors[i][2]);
        SDL_RenderCopyEx(renderer, duck_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
        SDL_SetTextureColorMod(duck_texture, 255, 255, 255); //reseteo el color

        if(duck.armor_equiped != 0){
            SDL_Rect armor_rect = transformToCameraSpace(
                duck.x + TILE_SIZE * 2,
              (duck.y  + TILE_SIZE * 6),
              (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
              (TILE_SIZE * DUCK_SIZE_Y / 2));
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &armor_rect, 0, NULL, duck.flipType);
        }

        if (duck.weapon_equiped != 0) {
            // Para renderizar el arma en la mano del pato
            double angle = 0.0;

            // Si el pato está mirando hacia arriba
            if (duck.is_looking_up) {
                angle = (duck.flipType == SDL_FLIP_HORIZONTAL) ? 90.0 : -90.0;
                gun_rect.x = (duck.flipType == SDL_FLIP_HORIZONTAL) ? gun_rect.x - TILE_SIZE * 3 : gun_rect.x + TILE_SIZE * 3;
            }

            if (duck.weapon_equiped == COWBOY_PISTOL_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("cowboy-pistol"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == LASER_RIFLE_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("laser-rifle"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == MAGNUM_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("magnum"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == PEW_PEW_LASER_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("pew-pew-laser"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == SHOTGUN_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("shotgun"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == AK_47_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("AK-47"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == SNIPER_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("sniper"), NULL, &gun_rect, angle, NULL, duck.flipType);
            } else if (duck.weapon_equiped == DUEL_PISTOL_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("duel-pistol"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == GRENADE_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("grenade-pin"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else if (duck.weapon_equiped == BANANA_ID) {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("banana"), NULL, &gun_rect, 0, NULL, duck.flipType);
            } else {
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("gun"), NULL, &gun_rect, angle, NULL, duck.flipType);
            }
        }

        if(duck.item_on_hand != 0){
            if (duck.item_on_hand == HELMET_ID){
                SDL_Rect helmet_rect = transformToCameraSpace(
                    duck.x + TILE_SIZE * 6,
                    (duck.y + TILE_SIZE * 2),
                    (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                    (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE);
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("helmet"), NULL, &helmet_rect, 0, NULL, duck.flipType);
            }
            
            if (duck.item_on_hand == ARMOR_ID){
                SDL_Rect armor_rect = transformToCameraSpace(
                    duck.x + TILE_SIZE * 6 + TILE_SIZE * 3,
                    (duck.y + TILE_SIZE * 2),
                    (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                    (TILE_SIZE * DUCK_SIZE_Y / 2));
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &armor_rect, 0, NULL, duck.flipType);
            }
        }

        if(duck.helmet_equiped != 0){
            SDL_Rect helmet_rect = transformToCameraSpace(
                duck.x + TILE_SIZE * 2,
                (duck.y - TILE_SIZE/4) - TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE);

            SDL_RenderCopyEx(renderer, texture_handler.getTexture("helmet"), NULL, &helmet_rect, 0, NULL, duck.flipType);
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
                SDL_Rect box_rect = transformToCameraSpace(
                        box.x,
                        box.y,
                        TILE_SIZE * BOX_SIZE_X,
                        TILE_SIZE * BOX_SIZE_Y );
            
                SDL_RenderCopyEx(renderer, texture_handler.getTexture("box"), NULL, &box_rect, 0, NULL, SDL_FLIP_NONE);

            } else if (!box.item_taked) {
                renderItem(box.item_id, box.x, box.y + 20);
            }
        }
}

void RendererManager::renderItem(uint8_t item_id, int x, int y, int mult, bool enableOscillation) {
    // Incrementar el tiempo solo si la oscilación está habilitada
    if (enableOscillation) {
        float_time += 0.05f;
    }

    // Calcular el movimiento oscilante solo si está habilitado
    float offset_y = enableOscillation ? std::sin(float_time) * 5 : 0;

    SDL_Rect rect_helmet = transformToCameraSpace(x, y - TILE_SIZE * 9 + static_cast<int>(offset_y), ((TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE) * mult, ((TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE) * mult);
    SDL_Rect rect_armor = transformToCameraSpace(x - TILE_SIZE, y - TILE_SIZE * 8 + static_cast<int>(offset_y), (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE, (TILE_SIZE * DUCK_SIZE_Y / 2) * mult);

    SDL_Rect rect_gun;
    if (enableOscillation) {
        rect_gun = transformToCameraSpace(x, y - TILE_SIZE * 11 + static_cast<int>(offset_y), TILE_SIZE * DUCK_SIZE_X * mult, TILE_SIZE * DUCK_SIZE_Y * mult);
    } else {
        rect_gun = {x, y - TILE_SIZE * 11 + static_cast<int>(offset_y), TILE_SIZE * DUCK_SIZE_X * mult, TILE_SIZE * DUCK_SIZE_Y * mult};
    }

    if (item_id == BASE_WEAPON_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("gun"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == COWBOY_PISTOL_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("cowboy-pistol"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == LASER_RIFLE_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("laser-rifle"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == MAGNUM_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("magnum"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == PEW_PEW_LASER_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("pew-pew-laser"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == SHOTGUN_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("shotgun"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == AK_47_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("AK-47"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == SNIPER_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("sniper"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == DUEL_PISTOL_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("duel-pistol"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == GRENADE_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("grenade-pin"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == BANANA_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("banana"), NULL, &rect_gun, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == HELMET_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("helmet"), NULL, &rect_helmet, 0, NULL, SDL_FLIP_NONE);
    } else if (item_id == ARMOR_ID) {
        SDL_RenderCopyEx(renderer, texture_handler.getTexture("armor"), NULL, &rect_armor, 0, NULL, SDL_FLIP_NONE);
    }
}

void RendererManager::renderItems(GameState* game) {
    for (auto& spawn_place : game->spawn_places) {
        // Acá reemplazar con los ids de las otras armas cuando tengas los renders de las mismas
        renderItem(spawn_place.item_id, spawn_place.x, spawn_place.y);
    }
}

void RendererManager::renderItemsOnFloor(GameState* game){
    for (auto& item_on_floor : game->items_on_floor) {
        // Acá reemplazar con los ids de las otras armas cuando tengas los renders de las mismas
        renderItem(item_on_floor.item_id, item_on_floor.x, item_on_floor.y + TILE_SIZE * 9); //eso en y porque la funcion de render item la hicimos para poner encima de un spawn place
    }
}

void RendererManager::renderThrowed(GameState* game) {
    for (auto& throwed_item : game->throwed_items) {
        //if (throwed_item.used)
        //    continue;
        // Para renderizar en el spawn_place
        if (throwed_item.type == GRENADE_ID) {
            throwed_item.used = true;
            SDL_Rect helmet_rect = transformToCameraSpace(
                throwed_item.current_x,
                throwed_item.current_y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE);
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("grenade"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        }
        if (throwed_item.type == BANANA_ID) {
            SDL_Rect helmet_rect = transformToCameraSpace(
                throwed_item.current_x,
                throwed_item.current_y - TILE_SIZE * 9,
                (TILE_SIZE * DUCK_SIZE_X / 2) + TILE_SIZE,
                (TILE_SIZE * DUCK_SIZE_Y / 2) + TILE_SIZE);
            SDL_RenderCopyEx(renderer, texture_handler.getTexture("banana-floor"), NULL, &helmet_rect, 0, NULL, SDL_FLIP_NONE);
        }
    }
    for (auto it = game->throwed_items.begin(); it != game->throwed_items.end();) {
        if (it->used || !it->touching_floor) {
            it = game->throwed_items.erase(it);
        } else {
            ++it;
        }
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

    if (!duck.is_dead) {
        SDL_Texture* heart_texture = texture_handler.getTexture("corazon");

        int heart_positions = 1;
        if (duck.armor_equiped || duck.helmet_equiped) {
            heart_positions++;
        }
        if (duck.armor_equiped && duck.helmet_equiped) {
            heart_positions++;
        }

        for (int i = 0; i < heart_positions; i++) {
            SDL_Rect heart_rect = {
                60 + (25 * i),
                30,
                TILE_SIZE * 6,
                TILE_SIZE * 6
            };
            SDL_RenderCopyEx(renderer, heart_texture, NULL, &heart_rect, 0, NULL, SDL_FLIP_NONE);
        }

        renderItem(duck.weapon_equiped, WINDOW_WIDTH - 90 , 15, 2, false);

        SDL_Rect bulletRect = {WINDOW_WIDTH - 140 , 10, 50, 50};
        SDL_Texture* bulletTexture = texture_handler.getTexture("bullet");

        for (int i = 0; i < duck.current_ammo; ++i) {
            SDL_RenderCopyEx(renderer, bulletTexture, NULL, &bulletRect, 270, NULL, SDL_FLIP_NONE);
            bulletRect.x -= 10;
        }
    } else {
        SDL_Rect grave_rect = {
            60,
            30,
            TILE_SIZE * 6,
            TILE_SIZE * 6
        };
        SDL_Texture* grave = texture_handler.getTexture("grave");
        SDL_RenderCopyEx(renderer, grave, NULL, &grave_rect, 0, NULL, SDL_FLIP_NONE);
    }
}

void RendererManager::doRenderDynamic(GameState* game, Message& message, uint16_t id) {

    updateCamera(game, camera);
    SDL_Rect src_rect = {
        static_cast<int>(camera.getX()),
        static_cast<int>(camera.getY()),
        camera.getWidth(),
        camera.getHeight()
    };

    SDL_Rect dest_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderCopy(renderer, texture_handler.getTexture("static_scene"), &src_rect, &dest_rect);

    renderDucks(game);
    renderItems(game);
    renderItemsOnFloor(game);
    renderBoxes(game);
    if (message.type == BULLET_POS_UPDATE){
        renderBullet(game);
    }
    renderThrowed(game);

    renderStats(game, id);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void RendererManager::updateCamera(GameState* game, Camera& camera) {
    if (game->ducks_quantity == 0) return;

    // Obtengo posiciones mínimas y máximas de los patos
    int min_x = WINDOW_WIDTH, min_y = WINDOW_HEIGHT;
    int max_x = 0, max_y = 0;

    for (int i = 0; i < game->ducks_quantity; i++) {
        Duck& duck = game->ducks[i];
        if (duck.is_dead) continue;

        min_x = std::min(min_x, duck.x);
        min_y = std::min(min_y, duck.y);
        max_x = std::max(max_x, duck.x);
        max_y = std::max(max_y, duck.y);
    }

    min_x = std::max(0, min_x - PADDING);
    min_y = std::max(0, min_y - PADDING);
    max_x = std::min(WINDOW_WIDTH, max_x + PADDING);
    max_y = std::min(WINDOW_HEIGHT, max_y + PADDING);

    int bounding_width = max_x - min_x;
    int bounding_height = max_y - min_y;

    float zoom_x = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(bounding_width);
    float zoom_y = static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(bounding_height);
    float new_zoom = std::min(zoom_x, zoom_y);
    new_zoom = std::clamp(new_zoom, MIN_ZOOM, MAX_ZOOM);

    int center_x = std::clamp(min_x + (max_x - min_x) / 2, camera.getWidth() / 2, WINDOW_WIDTH - camera.getWidth() / 2);
    int center_y = std::clamp(min_y + (max_y - min_y) / 2, camera.getHeight() / 2, WINDOW_HEIGHT - camera.getHeight() / 2);

    camera.setZoom(camera.getZoom() + SMOOTH_FACTOR * (new_zoom - camera.getZoom()));
    camera.setPosition(
        camera.getX() + SMOOTH_FACTOR * ((center_x - camera.getWidth() / 2) - camera.getX()),
        camera.getY() + SMOOTH_FACTOR * ((center_y - camera.getHeight() / 2) - camera.getY())
    );
}
