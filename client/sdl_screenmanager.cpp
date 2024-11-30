#include "sdl_screenmanager.h"

#include "common/constants.h"
#include <algorithm>

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

void ScreenManager::showGetReadyScreen() {
    SDL_Texture* static_scene = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!static_scene) {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderTarget(renderer, static_scene);
    SDL_RenderClear(renderer); // Limpiar el renderizador antes de dibujar

    SDL_RenderCopy(renderer, getTexture("next-round-background"), NULL, NULL);

    texture_handler.saveText("04B_16", "GET READY!", {255, 255, 255, 255});

    SDL_Point t_size;
    SDL_QueryTexture(texture_handler.getText("GET READY!"), NULL, NULL, &t_size.x, &t_size.y);
    SDL_Rect textRect = {WINDOW_WIDTH / 2 - t_size.x / 2, WINDOW_HEIGHT / 2 - t_size.y / 2, t_size.x, t_size.y};
    SDL_RenderCopy(renderer, texture_handler.getText("GET READY!"), NULL, &textRect);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, static_scene, NULL, NULL); 
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    SDL_DestroyTexture(static_scene);
}


void ScreenManager::showNextRoundScreen(uint16_t id_winner) {
    if (id_winner < 1 || id_winner > 6) return;

    // Dibujar fondo
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, getTexture("next-round-background"), NULL, NULL);
    SDL_RenderPresent(renderer);

    // Configurar parámetros del pato
    int pos_id = id_winner - 1;
    SDL_Texture* duck_texture = texture_handler.getTexture("duck");
    SDL_SetTextureColorMod(duck_texture, colors[pos_id][0], colors[pos_id][1], colors[pos_id][2]);

    // Tamaño y posición final del pato (agrandado)
    int final_width = 7 * TILE_SIZE * DUCK_SIZE_X;
    int final_height = 7 * TILE_SIZE * DUCK_SIZE_Y;
    int center_x = (WINDOW_WIDTH - final_width) / 2;
    int center_y = 100;

    // Animación de escalado del pato
    for (float scale = 0.1f; scale <= 1.0f; scale += 0.05f) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, getTexture("next-round-background"), NULL, NULL);

        int width = static_cast<int>(final_width * scale);
        int height = static_cast<int>(final_height * scale);
        SDL_Rect duck_rect = {center_x + (final_width - width) / 2, center_y + (final_height - height) / 2, width, height};

        SDL_RenderCopyEx(renderer, duck_texture, NULL, &duck_rect, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    SDL_SetTextureColorMod(duck_texture, 255, 255, 255); // Restaurar color

    SDL_Delay(500); // Pausa antes del texto

    // Dibujar texto debajo del pato
    texture_handler.saveText("8bit_bigger", "Winner", {255, 255, 255, 255});
    SDL_Point t_size;
    SDL_QueryTexture(texture_handler.getText("Winner"), NULL, NULL, &t_size.x, &t_size.y);

    SDL_Rect textRect = {
        (WINDOW_WIDTH - t_size.x) / 2,          // Centramos horizontalmente
        center_y + final_height + 40,           // Más separado del pato
        t_size.x,
        t_size.y
    };

    SDL_RenderCopy(renderer, texture_handler.getText("Winner"), NULL, &textRect);
    SDL_RenderPresent(renderer);

    SDL_Delay(2000);
}

void ScreenManager::showScoreboard(std::vector<int> scoreboard) {
    std::vector<DuckScore> duck_scores = sortScoreboard(scoreboard);

    // Crear textura para la escena estática
    SDL_Texture* static_scene = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!static_scene) {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetRenderTarget(renderer, static_scene);
    SDL_RenderClear(renderer);

    // Dibujar fondo
    SDL_RenderCopy(renderer, getTexture("screen"), NULL, NULL);

    // Dibujar título "Scoreboard"
    texture_handler.saveText("8bit_bigger", "Scoreboard", {255, 255, 255, 255});
    SDL_Point t_size;
    SDL_QueryTexture(texture_handler.getText("Scoreboard"), NULL, NULL, &t_size.x, &t_size.y);
    SDL_Rect textRect = {WINDOW_WIDTH / 2 - t_size.x / 2, 100, t_size.x, t_size.y};
    SDL_RenderCopy(renderer, texture_handler.getText("Scoreboard"), NULL, &textRect);

    // Variables para los podios y patos
    SDL_Texture* duck_texture = texture_handler.getTexture("duck");
    SDL_Texture* crate_texture = texture_handler.getTexture("crate");

    // Dimensiones ajustadas
    int podio_base_height = 20 * TILE_SIZE;    // Altura base del podio (doble)
    int podio_decremento = 5 * TILE_SIZE;      // Decremento mayor (doble)
    int duck_width = 2 * TILE_SIZE * DUCK_SIZE_X;
    int duck_height = 2 * TILE_SIZE * DUCK_SIZE_Y;
    int spacing = 200;                         // Más espacio entre patos/podios
    int start_x = (WINDOW_WIDTH - (duck_scores.size() * spacing)) / 2;
    int base_y = 500;                          // Posición inicial más baja

    for (size_t i = 0; i < duck_scores.size(); ++i) {
        DuckScore& score = duck_scores[i];

        // Calcular posición del podio
        int podio_height = podio_base_height - (i * podio_decremento);
        int podio_x = start_x + (i * spacing);
        int podio_y = base_y - podio_height;

        // Dibujar podio
        SDL_Rect podio_rect = {podio_x, podio_y, duck_width, podio_height};
        SDL_RenderCopy(renderer, crate_texture, NULL, &podio_rect);

        // Dibujar pato encima del podio
        SDL_SetTextureColorMod(duck_texture, colors[score.id][0], colors[score.id][1], colors[score.id][2]);
        SDL_Rect duck_rect = {podio_x, podio_y - duck_height, duck_width, duck_height};
        SDL_RenderCopyEx(renderer, duck_texture, NULL, &duck_rect, 0, NULL, SDL_FLIP_NONE);

        // Dibujar rondas ganadas (texto blanco, centrado)
        std::string score_text = std::to_string(score.rounds_won);
        texture_handler.saveText("8bit_bigger", score_text, {255, 255, 255, 255});
        SDL_QueryTexture(texture_handler.getText(score_text), NULL, NULL, &t_size.x, &t_size.y);
        SDL_Rect scoreRect = {podio_x + (duck_width - t_size.x) / 2, podio_y - duck_height - t_size.y - 20, t_size.x, t_size.y};
        SDL_RenderCopy(renderer, texture_handler.getText(score_text), NULL, &scoreRect);
    }

    // Renderizar escena final
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, static_scene, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Mostrar la pantalla por unos segundos
    SDL_Delay(9000);
    SDL_DestroyTexture(static_scene);
}




std::vector<DuckScore> ScreenManager::sortScoreboard(std::vector<int> scoreboard) {
    std::vector<DuckScore> duck_scores;
    for (size_t i = 0; i < scoreboard.size(); ++i) {
        duck_scores.emplace_back(i, scoreboard[i]);
    }

    std::sort(duck_scores.begin(), duck_scores.end(), [](const DuckScore& a, const DuckScore& b) {
        return a.rounds_won > b.rounds_won; // Ordena de mayor a menor
    });

    return duck_scores;
}


void ScreenManager::loadLobbyScreen() {
    std::unordered_map<std::string, std::string> textures_to_load = {
                        {"background", "start/galaxy"},
                        {"start-button", "start/start-match"},
                        {"new-match-button", "start/new-match"},
                        {"join-button", "start/join-match"},
                        {"next-round-background", "round/nightsky"},
                        {"screen", "round/screen"},
                        {"duck", "duck"}};

    // Cargar imagenes del lobby
    for (auto& texture : textures_to_load) {
        SDL_Texture* sdl_texture = texture_handler.loadSimpleTexture(texture.second);
        lobby_textures[texture.first] = sdl_texture;
    }
    texture_handler.loadFont("04B_16", "04B_30__.TTF", 90);
    texture_handler.loadFont("8bit", "8bitOperatorPlus8-Regular.ttf", 25);
    texture_handler.loadFont("8bit_bigger", "8bitOperatorPlus8-Regular.ttf", 70);

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