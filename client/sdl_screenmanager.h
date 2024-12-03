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
    void showEndMatchScreen(uint16_t id_winner);
    void showServerIsDownScreen();
    void showGetReadyScreen(int round);
    void showScoreboard(std::vector<int> scoreboard, bool end);
    void loadLobbyScreen();
    void renderStaticLobby();
    void showLobbyScreen();
    SDL_Texture* getTexture(const std::string& name) const;
    void renderNewMatchText(int id_match);
    void renderAvailableMatches(std::vector<uint16_t> existing_matches);
    Button* getButton(uint8_t id);
    void renderSelectedMatch(int x, int y, int& chosen_match);
    void renderChosenMatch();

private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
    std::vector<Button> buttons;
    std::unordered_map<std::string, SDL_Texture*> lobby_textures;
    std::vector<DuckScore> sortScoreboard(std::vector<int> scoreboard);
    std::vector<std::pair<SDL_Rect, uint16_t>> matches;
    int selected_match_id = 0;
    SDL_Rect selected_match_rect;
};

#endif //SDL_SCREENMANAGER_H
