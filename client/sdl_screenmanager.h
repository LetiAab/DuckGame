#ifndef SDL_SCREENMANAGER_H
#define SDL_SCREENMANAGER_H
#include "sdl_texturehandler.h"

struct Button {
    uint8_t id;
    int x, y;
    int w, h;
};

class ScreenManager {
public:
    ScreenManager(SDL_Renderer *renderer, TextureHandler &texture_handler);
    ~ScreenManager();
    void showStartScreen();
    void showLobbyScreen();
    void loadLobbyScreen();
    SDL_Texture* getTexture(const std::string& name) const;
    void renderNewMatchText();
    void renderAvailableMatches();
    Button* getButton(uint8_t id);
private:
    SDL_Renderer* renderer;
    TextureHandler& texture_handler;
    std::vector<Button> buttons;
    std::unordered_map<std::string, SDL_Texture*> lobby_textures;
};

#endif //SDL_SCREENMANAGER_H
