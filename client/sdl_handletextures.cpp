#include "sdl_handletextures.h"
#include <SDL2/SDL_image.h>
#include "common/constants.h"

SDLHandleTextures::SDLHandleTextures(SDL_Renderer* renderer): renderer(renderer) {}

SDL_Surface* SDLHandleTextures::loadImage(const std::string& name_img) {
    SDL_Surface* img = NULL;
    const std::string path = std::string(IMAGE_PATH) + name_img + ".png";
    img = IMG_Load(path.c_str());
    if (img == NULL) {
        std::cerr << "Error loading " << name_img << ".png: " << IMG_GetError() << "\n";
        SDL_Quit();
        exit(ERROR);
    }
    return img;
}

SDL_Texture* SDLHandleTextures::loadSimpleTexture(const std::string& name_img) {
    SDL_Surface* img = loadImage(name_img);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return texture;
}

SpriteSheet SDLHandleTextures::loadSpriteSheet(const std::string& name_img) {
    SDL_Texture* texture = loadSimpleTexture(name_img);
    int sprite_sheet_width, sprite_sheet_height;
    SDL_QueryTexture(texture, NULL, NULL, &sprite_sheet_width, &sprite_sheet_height);
    const int frame_width = sprite_sheet_width / 6;  // 6 fotogramas en una fila
    const int frame_height = sprite_sheet_height;    // Solo una fila

    return {texture, frame_width, frame_height};
}

void SDLHandleTextures::saveTexture(const std::string& name, SDL_Texture* texture) {
    textures.insert({name, texture});
}

SDL_Texture* SDLHandleTextures::getTexture(const std::string& name) const {
    return textures.at(name);
}

void SDLHandleTextures::destroyTextures() {
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}