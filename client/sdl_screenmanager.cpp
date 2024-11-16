#include "sdl_screenmanager.h"

#include "common/constants.h"

ScreenManager::ScreenManager(SDL_Renderer* renderer, TextureHandler& texture_handler) :
    renderer(renderer), texture_handler(texture_handler) {}

void ScreenManager::showStartScreen() {
    SDL_Delay(1); //cambiar a 1000
    SDL_Texture* start_logo = texture_handler.loadSimpleTexture("start/duckgame_logo");
    SDL_Point size;
    SDL_QueryTexture(start_logo, NULL, NULL, &size.x, &size.y);
    SDL_Rect start_logo_rect = {WINDOW_WIDTH/2-size.x/2, WINDOW_HEIGHT/2-size.y/2, size.x, size.y};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, start_logo, NULL, &start_logo_rect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2); // cambiar a 2000
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

void ScreenManager::showLobbyScreen() {
    SDL_Texture* start_background = texture_handler.loadSimpleTexture("start/galaxy");
    SDL_RenderCopy(renderer, start_background, NULL, NULL);

    SDL_Texture* textureText = texture_handler.getText("Lobby", {255, 255, 255, 255});

    int size_x = 400, size_y = 100;
    SDL_Rect textRect = {WINDOW_WIDTH/2-size_x/2, 20, size_x, size_y};
    SDL_RenderCopy(renderer, textureText, NULL, &textRect);

    SDL_Texture* start_button = texture_handler.loadSimpleTexture("start/play");
    SDL_Rect start_button_rect = {WINDOW_WIDTH/2-100, WINDOW_HEIGHT/2-50, 200, 100};
    SDL_RenderCopy(renderer, start_button, NULL, &start_button_rect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(start_background);
    SDL_DestroyTexture(textureText);
}