#ifndef SDL_TEXTURES_H
#define SDL_TEXTURES_H
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common/constants.h"
#include <iostream>
#include <unordered_map>

struct SpriteSheet {
    SDL_Texture* texture;
    int frame_width;
    int frame_height;
};

class SDLHandleTextures {
public:
    SDLTextures(SDL_Renderer* renderer);
    SDL_Texture* loadSimpleTexture(const std::string& name_img);
    SpriteSheet loadSpriteSheet(const std::string& name_img);

    /*SDL_Texture* getBackground();
    SDL_Texture* getDuck();
    SDL_Texture* getCrate();*/
    //~SDLTextures();

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
    SDL_Surface* loadImage(const std::string& name_img);
    //SDL_Texture *background, *walk_duck, *crate;
    /*void loadBackground();
    void loadDuck();
    void loadCrate();*/
};



#endif //SDL_TEXTURES_H
