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
    texture_handler.loadFont("04B_16", "04B_30__.TTF", 90);
    texture_handler.loadFont("8bit", "8bitOperatorPlus8-Regular.ttf", 25);

    texture_handler.saveText("04B_16", "Lobby", {255, 255, 255, 255});
    texture_handler.saveText("8bit", "Match 1 Created", {255, 255, 255, 255});
}

SDL_Texture* ScreenManager::getTexture(const std::string& name) const {
    return lobby_textures.at(name);
}

void ScreenManager::showLobbyScreen() {
    SDL_RenderCopy(renderer, getTexture("background"), NULL, NULL);

    // Titulo Lobby
    SDL_Point t_size;
    SDL_QueryTexture(texture_handler.getText("Lobby"), NULL, NULL, &t_size.x, &t_size.y);
    SDL_Rect textRect = {WINDOW_WIDTH/2-t_size.x/2, 20, t_size.x, t_size.y};
    SDL_RenderCopy(renderer, texture_handler.getText("Lobby"), NULL, &textRect);

    // BOTONES
    Button start = {START_MATCH_CODE, WINDOW_WIDTH/2-BUTTON_W/2, WINDOW_HEIGHT-120, BUTTON_W, BUTTON_H};
    buttons.push_back(start);
    SDL_Rect start_button_rect = {start.x, start.y, start.w, start.h};
    SDL_RenderCopy(renderer, getTexture("start-button"), NULL, &start_button_rect);

    Button new_match = {NEW_MATCH_CODE, 80, t_size.y+80, BUTTON_W, BUTTON_H};
    buttons.push_back(new_match);
    SDL_Rect new_match_button_rect = {new_match.x, new_match.y, new_match.w, new_match.h};
    SDL_RenderCopy(renderer, getTexture("new-match-button"), NULL, &new_match_button_rect);

    Button join = {LIST_MATCH_AVAILABLE, WINDOW_WIDTH-BUTTON_W-80, t_size.y+80, BUTTON_W, BUTTON_H};
    buttons.push_back(join);
    SDL_Rect join_button_rect = {join.x, join.y, join.w, join.h};
    SDL_RenderCopy(renderer, getTexture("join-button"), NULL, &join_button_rect);

    SDL_RenderPresent(renderer);
}

Button* ScreenManager::getButton(const uint8_t id) {
    for (auto& button : buttons) {
        if (button.id == id) {
            return &button;
        }
    }
    throw std::runtime_error("Button not found");
}

void ScreenManager::renderNewMatchText() {
    SDL_Point size;
    SDL_QueryTexture(texture_handler.getText("Match 1 Created"), NULL, NULL, &size.x, &size.y);
    SDL_Rect newRect = {90, 280, size.x, size.y};
    SDL_RenderCopy(renderer, texture_handler.getText("Match 1 Created"), NULL, &newRect);
    SDL_RenderPresent(renderer);
}

void ScreenManager::renderAvailableMatches() {
    Button* join = getButton(LIST_MATCH_AVAILABLE);
    SDL_Rect rect = {join->x+30, join->y+join->h+30, 30, 30};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

ScreenManager::~ScreenManager() {
    for (auto& texture : lobby_textures) {
        SDL_DestroyTexture(texture.second);
    }
}