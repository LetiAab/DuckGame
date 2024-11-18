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
}

SDL_Texture* ScreenManager::getTexture(const std::string& name) const {
    return lobby_textures.at(name);
}

void ScreenManager::renderStaticLobby() {
    SDL_Texture* static_scene = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderTarget(renderer, static_scene);

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

    SDL_SetRenderTarget(renderer, NULL);
    lobby_textures["static_scene"] = static_scene;
}

Button* ScreenManager::getButton(const uint8_t id) {
    for (auto& button : buttons) {
        if (button.id == id) {
            return &button;
        }
    }
    throw std::runtime_error("Button not found");
}

void ScreenManager::showLobbyScreen() {
    //SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, lobby_textures["static_scene"], NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ScreenManager::renderNewMatchText(int id_match) {
    std::string text = "Match " + std::to_string(id_match) + " Created";
    texture_handler.saveText("8bit", text, {255, 255, 255, 255});

    SDL_Point size;
    SDL_QueryTexture(texture_handler.getText(text), NULL, NULL, &size.x, &size.y);
    SDL_Rect newRect = {90, 280, size.x, size.y};

    SDL_RenderCopy(renderer, lobby_textures["static_scene"], NULL, NULL);

    SDL_RenderCopy(renderer, texture_handler.getText(text), NULL, &newRect);
    SDL_RenderPresent(renderer);
}

void ScreenManager::renderAvailableMatches(int id_match) {
    SDL_RenderCopy(renderer, lobby_textures["static_scene"], NULL, NULL);
    Button* join = getButton(LIST_MATCH_AVAILABLE);
    int rect_x = join->x+30;
    int rect_y = join->y+join->h+30;
    for (int i=1; i<id_match; i++) {
        if(rect_x+50 >= WINDOW_WIDTH) {
            rect_x = join->x+30;
            rect_y += 50;
        }
        SDL_Rect rect = {rect_x, rect_y, 40, 40};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        texture_handler.saveText("8bit", std::to_string(i), {255, 255, 255, 255});
        SDL_RenderCopy(renderer, texture_handler.getText(std::to_string(i)), NULL, &rect);
        rect_x += 50;
    }

    SDL_RenderPresent(renderer);
}

ScreenManager::~ScreenManager() {
    for (auto& texture : lobby_textures) {
        SDL_DestroyTexture(texture.second);
    }
}