#include "sdl_texturehandler.h"
#include <SDL2/SDL_image.h>
#include "common/constants.h"

TextureHandler::TextureHandler(SDL_Renderer* renderer): renderer(renderer) {}

SDL_Surface* TextureHandler::loadImage(const std::string& name_img) {
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

SDL_Texture* TextureHandler::loadSimpleTexture(const std::string& name_img) {
    SDL_Surface* img = loadImage(name_img);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return texture;
}

SpriteSheet TextureHandler::loadSpriteSheet(const std::string& name_img) {
    SDL_Texture* texture = loadSimpleTexture(name_img);
    int sprite_sheet_width, sprite_sheet_height;
    SDL_QueryTexture(texture, NULL, NULL, &sprite_sheet_width, &sprite_sheet_height);
    const int frame_width = sprite_sheet_width / 6;  // 6 fotogramas en una fila
    const int frame_height = sprite_sheet_height;    // Solo una fila

    return {texture, frame_width, frame_height};
}

void TextureHandler::saveTexture(const std::string& name, SDL_Texture* texture) {
    textures.insert({name, texture});
}

SDL_Texture* TextureHandler::getTexture(const std::string& name) const {
    return textures.at(name);
}

// Creo un render target para dibujar en él todos los objetos estaticos
SDL_Texture* TextureHandler::createRenderTarget(const std::string& name, int width, int height) {
    if (!renderer) {
        std::cerr << "Renderer is not initialized!\n";
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture) {
        std::cerr << "Error creating render target texture: " << SDL_GetError() << "\n";
        SDL_Quit();
        exit(ERROR);
    }
    saveTexture(name, texture);
    return texture;
}

void TextureHandler::destroyTextures() {
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}