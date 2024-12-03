#include "sdl_texturehandler.h"
#include <SDL2/SDL_image.h>
#include "SDL2pp/Font.hh"
#include "common/constants.h"

TextureHandler::TextureHandler(SDL_Renderer* renderer): renderer(renderer) {
    if(TTF_Init() == -1) {
        std::cout << "Could not initialize SDL2 TTF: " << TTF_GetError() << "\n";
        SDL_Quit();
        exit(ERROR);
    }
}

//** TEXTURES IMAGES **//
SDL_Surface* TextureHandler::loadImage(const std::string& name_img) {
    const std::string path = std::string(IMAGE_PATH) + name_img + ".png";
    SDL_Surface* img = IMG_Load(path.c_str());
    if (!img) {
        std::cerr << "Error loading " << name_img << ".png: " << IMG_GetError() << "\n";
        SDL_Quit();
        exit(ERROR);
    }
    return img;
}

void TextureHandler::loadTexture(const TextureInfo& texture_info, int* frame_width, int* frame_height) {
    SDL_Texture *texture = loadSimpleTexture(texture_info.path);

    if (texture_info.frames > 1) {
        SDL_QueryTexture(texture, NULL, NULL, frame_width, frame_height);
        *frame_width /= texture_info.frames;
    }

    saveTexture(texture_info.name, texture);
}

SDL_Texture* TextureHandler::loadSimpleTexture(const std::string& path) {
    SDL_Surface* img = loadImage(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, img);
    SDL_FreeSurface(img);
    return texture;
}

void TextureHandler::saveTexture(const std::string& name, SDL_Texture* texture) {
    textures[name] = texture;
}

SDL_Texture* TextureHandler::getTexture(const std::string& name) const {
    return textures.at(name);
}

//** RENDER TARGET **//
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

//** FONTS **//
void TextureHandler::loadFont(const std::string& name, const std::string& font_file, int font_size) {
    const std::string path = std::string(FONT_PATH) + font_file;
    TTF_Font* font = TTF_OpenFont(path.c_str(), font_size);
    if (!font) {
        std::cerr << "Could not load font " << font_file << ": " << TTF_GetError() << "\n";
        SDL_Quit();
        exit(ERROR);
    }
    fonts[name] = font;
}

void TextureHandler::saveText(const std::string& fname, const std::string& text, const SDL_Color color) {
    SDL_Surface* surfaceText = TTF_RenderText_Solid(fonts[fname], text.c_str(), color);
    SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    textures[text] = textureText;
}

SDL_Texture* TextureHandler::getText(const std::string& text) const {
    return textures.at(text);
}

//** DESTRUCTOR **//
TextureHandler::~TextureHandler() {
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();

    for (auto& font : fonts) {
        TTF_CloseFont(font.second);
    }
    TTF_Quit();
}