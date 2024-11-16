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

    SDL_SetRenderTarget(renderer, NULL);
}

//** Dinamico **//
void RendererManager::renderBullet(const int x, const int y, const int size) {
    SDL_Rect bulletRect = { x * TILE_SIZE, y * TILE_SIZE, size, size };
    SDL_RenderCopy(renderer, texture_handler.getTexture("bullet"), NULL, &bulletRect);
}

void RendererManager::renderDucks(GameState* game, Message& message) {
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
        if (message.is_jumping && !message.is_fluttering) {
            duck_texture = texture_handler.getTexture("duck-jumping");
            wings_texture = texture_handler.getTexture("duck-jumping-wings");
        } else if (message.is_fluttering) {
            duck_texture = texture_handler.getTexture("duck-fluttering");
            //wings_texture = texture_handler.getTexture("duck-fluttering-wings");
        } else {
            duck_texture = texture_handler.getTexture("duck-walking");
            wings_texture = texture_handler.getTexture("duck-walking-wings");
        }

        SDL_SetTextureColorMod(duck_texture, colors[i][0], colors[i][1], colors[i][2]);
        SDL_RenderCopyEx(renderer, duck_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
        SDL_SetTextureColorMod(duck_texture, 255, 255, 255); //reseteo el color

        SDL_RenderCopyEx(renderer, texture_handler.getTexture("gun"), NULL, &gun_rect, 0, NULL, duck.flipType);

        if (wings_texture != nullptr) {
            SDL_SetTextureColorMod(wings_texture, colors[i][0], colors[i][1], colors[i][2]);
            SDL_RenderCopyEx(renderer, wings_texture, &src_rect, &duck_rect, 0, NULL, duck.flipType);
            SDL_SetTextureColorMod(wings_texture, 255, 255, 255); //reseteo el color
        }

    }
}

void RendererManager::doRenderDynamic(GameState* game, Message& message) {
    SDL_RenderCopy(renderer, texture_handler.getTexture("static_scene"), NULL, NULL);
    renderDucks(game, message);
    if(message.type == BULLET_POS_UPDATE){
        renderBullet(message.bullet_x, message.bullet_y);
    }

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}