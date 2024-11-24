#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include <SDL.h>
#include <vector>
#include "common/constants.h"

struct Crate {
    int x, y;
    SDL_Texture* texture; // Texture for the crate
};

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor();

    bool init();
    void run();
    void cleanup();
    void drawDottedGrid();


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* crateTexture;

    std::vector<Crate> crates; // All placed crates
    SDL_Texture* selectedTexture; // Texture of the selected crate

    int crateX, crateY; // Position for the "buttons" in the tool area
    int selectedCrateIndex; // Index of the selected crate type (0: crate)

    enum Tool {
        NONE,
        CREATE_CRATE
    };

    Tool currentTool;
};

#endif // LEVEL_EDITOR_H
