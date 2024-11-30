#ifndef SDL_SCREENMANAGER_H
#define SDL_SCREENMANAGER_H
#include "sdl_texturehandler.h"
#include "game_state.h"

struct Button {
    uint8_t id;
    int x, y;
    int w, h;
};

struct DuckScore {
    int id;
    int rounds_won;
};

class ScreenManager {
public:
    ScreenManager(SDL_Renderer *renderer, TextureHandler &texture_handler);
    ~ScreenManager();
    void showStartScreen();
    void showNextRoundScreen(uint16_t id_winner);
    void showGetReadyScreen();
    void showScoreboard(std::vector<int> scoreboard);
    void loadLobbyScreen();
    void renderStaticLobby();
    void showLobbyScreen();
    SDL_Texture* getTexture(const std::string& name) const;
    void renderNewMatchText(int id_match);
    void renderAvailableMatches(int id_match);
    Button* getButton(uint8_t id);

private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
    std::vector<Button> buttons;
    std::unordered_map<std::string, SDL_Texture*> lobby_textures;

    std::vector<DuckScore> sortScoreboard(std::vector<int> scoreboard);
};

#endif //SDL_SCREENMANAGER_H
