#ifndef SDL_HANDLETEXTURES_H
#define SDL_HANDLETEXTURES_H
#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_map>

struct SpriteSheet {
    SDL_Texture* texture;
    int frame_width;
    int frame_height;
};

class SDLHandleTextures {
public:
    explicit SDLHandleTextures(SDL_Renderer* renderer);
    SDL_Texture* loadSimpleTexture(const std::string& name_img);
    SpriteSheet loadSpriteSheet(const std::string& name_img);
    void saveTexture(const std::string&, SDL_Texture* texture);
    SDL_Texture* getTexture(const std::string& name) const;
    void destroyTextures();

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
    SDL_Surface* loadImage(const std::string& name_img);
};


#endif //SDL_HANDLETEXTURES_H
