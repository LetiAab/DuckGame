#include "sdl_textures.h"

SDLTextures::SDLTextures(SDL_Renderer* renderer) {
    this->renderer = renderer;
}

SDL_Surface* SDLTextures::loadImage(const std::string& name_img) {
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

SDL_Texture* SDLTextures::loadSimpleTexture(const std::string& name_img) {
    SDL_Surface* img = loadImage(name_img);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return texture;
}

SpriteSheet SDLTextures::loadSpriteSheet(const std::string& name_img) {
    SDL_Texture* texture = loadSimpleTexture(name_img);
    int sprite_sheet_width, sprite_sheet_height;
    SDL_QueryTexture(texture, NULL, NULL, &sprite_sheet_width, &sprite_sheet_height);
    const int frame_width = sprite_sheet_width / 6;  // 6 fotogramas en una fila
    const int frame_height = sprite_sheet_height;    // Solo una fila

    return {texture, frame_width, frame_height};
}

/*void SDLTextures::loadBackground() {
    SDL_Surface* img = loadImage("background");
    background = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
}

void SDLTextures::loadDuck() {
    SDL_Surface* img = loadImage("duck-walking");
    walk_duck = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);

    // el sprite sheet tiene 6 fotogramas en una fila
    int sprite_sheet_width, sprite_sheet_height;
    SDL_QueryTexture(walk_duck, NULL, NULL, &sprite_sheet_width, &sprite_sheet_height);
    int frame_width = sprite_sheet_width / 6;  // 6 fotogramas en una fila
    int frame_height = sprite_sheet_height;    // Solo una fila
}

void SDLTextures::loadCrate() {
    SDL_Surface* img = loadImage("crate");
    crate = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
}*/