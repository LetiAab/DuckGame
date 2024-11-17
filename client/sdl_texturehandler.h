#ifndef SDL_TEXTUREHANDLER_H
#define SDL_TEXTUREHANDLER_H
#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include <unordered_map>

#include "SDL2pp/Font.hh"

struct SpriteSheet {
    SDL_Texture* texture;
    int frame_width;
    int frame_height;
};

struct TextureInfo {
    std::string name;
    std::string path;
    int frames;
};

class TextureHandler {
public:
    explicit TextureHandler(SDL_Renderer* renderer);
    ~TextureHandler();
    void loadTexture(const TextureInfo& texture_info, int* frame_width = nullptr, int* frame_height = nullptr);
    SDL_Texture* loadSimpleTexture(const std::string& path);
    SDL_Texture* getTexture(const std::string& name) const;
    SDL_Texture* createRenderTarget(const std::string& name, int width, int height);
    void loadFont(const std::string& name, const std::string& font_file, int font_size);
    void saveText(const std::string& fname, const std::string &text, SDL_Color color);
    SDL_Texture* getText(const std::string& text) const;

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, TTF_Font*> fonts;
    SDL_Surface* loadImage(const std::string& name_img);
    void saveTexture(const std::string&, SDL_Texture* texture);
};

#endif //SDL_TEXTUREHANDLER_H
