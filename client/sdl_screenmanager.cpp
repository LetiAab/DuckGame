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

void ScreenManager::loadLobbyScreen() {
    std::unordered_map<std::string, std::string> textures_to_load = {
                        {"background", "start/galaxy"},
                        {"start-button", "start/start-match"},
                        {"new-match-button", "start/new-match"},
                        {"join-button", "start/join-match"}};

    // Cargar imagenes del lobby
    for (auto& texture : textures_to_load) {
        SDL_Texture* sdl_texture = texture_handler.loadSimpleTexture(texture.second);
        lobby_textures[texture.first] = sdl_texture;
    }
    texture_handler.loadFont("04B_16", "04B_30__.TTF", 16);
    texture_handler.saveText("04B_16", "Lobby", {255, 255, 255, 255});
}

SDL_Texture* ScreenManager::getTexture(const std::string& name) const {
    return lobby_textures.at(name);
}

void ScreenManager::showLobbyScreen() {
    SDL_RenderCopy(renderer, getTexture("background"), NULL, NULL);

    // Titulo Lobby
    int size_x = 400, size_y = 100;
    SDL_Rect textRect = {WINDOW_WIDTH/2-size_x/2, 20, size_x, size_y};
    SDL_RenderCopy(renderer, texture_handler.getText("Lobby"), NULL, &textRect);

    // BOTONES
    SDL_Rect start_button_rect = {WINDOW_WIDTH/2-BUTTON_W/2, WINDOW_HEIGHT-120, BUTTON_W, BUTTON_H};
    SDL_RenderCopy(renderer, getTexture("start-button"), NULL, &start_button_rect);

    SDL_Rect new_match_button_rect = {80, size_y+80, BUTTON_W, BUTTON_H};
    SDL_RenderCopy(renderer, getTexture("new-match-button"), NULL, &new_match_button_rect);

    SDL_Rect join_button_rect = {WINDOW_WIDTH-BUTTON_W-80, size_y+80, BUTTON_W, BUTTON_H};
    SDL_RenderCopy(renderer, getTexture("join-button"), NULL, &join_button_rect);

    SDL_RenderPresent(renderer);
}

ScreenManager::~ScreenManager() {
    for (auto& texture : lobby_textures) {
        SDL_DestroyTexture(texture.second);
    }
}