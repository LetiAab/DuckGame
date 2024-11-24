#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#define GRID_CELL_SIZE 16 // Tamaño de cada celda (16 píxeles)
#define GRID_WIDTH (WINDOW_WIDTH / GRID_CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / GRID_CELL_SIZE)

#include <SDL.h>
#include <vector>
#include "common/constants.h"

struct Crate {
    int x, y;
    SDL_Texture* texture; // Texture for the crate
};

struct SpawnPlace {
    int x, y;
    SDL_Texture* texture; 
};

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor();

    bool init();
    void run();
    void cleanup();
    bool loadTextures();
    void handleEvent(SDL_Event& event, bool& running);
    void drawDottedGrid();
    void renderToolArea();
    void renderElements();


private:
    bool occupancyGrid[GRID_WIDTH][GRID_HEIGHT] = {false}; 
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* crateTexture;
    SDL_Texture* spawnPlaceTexture;

    std::vector<Crate> crates; // All placed crates
    std::vector<SpawnPlace> spawn_places;
    SDL_Texture* selectedTexture; // Texture of the selected crate

    int crateX, crateY; // Position for the "buttons" in the tool area
    int selectedCrateIndex; // Index of the selected crate type (0: crate)

    int spawnPlaceX, spawnPlaceY;
    int selectedSpawnPlaceIndex;

    enum Tool {
        NONE,
        CREATE_CRATE,
        CREATE_SPAWN_PLACE
    };

    Tool currentTool;
};

#endif // LEVEL_EDITOR_H
