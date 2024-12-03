#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#define GRID_CELL_SIZE 16 // Tamaño de cada celda (16 píxeles)
#define GRID_WIDTH (WINDOW_WIDTH / GRID_CELL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / GRID_CELL_SIZE)

#include <SDL.h>
#include <vector>
#include "../common/constants.h"
#include <unordered_map>
#include <string>
#include <SDL_ttf.h>

#include <fstream>
#include <iostream>

struct Crate {
    int x, y;
    SDL_Texture* texture; // Texture for the crate
};

struct SpawnPlace {
    int x, y;
    SDL_Texture* texture; 
};

struct Box {
    int x, y;
    SDL_Texture* texture; 
};

struct Duck {
    int x, y;
    SDL_Texture* texture; 
};

struct Item {
    int x,y;
    uint8_t item_id;
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
    void deleteElement(std::string key, int gridX, int gridY);
    void saveLevel();
    void uploadLevel(const std::string& filePath);
    SDL_Texture* getItemTexture(uint8_t itemId);


private:
    bool occupancyGrid[GRID_WIDTH][GRID_HEIGHT] = {false}; 
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;

    SDL_Texture* crateTexture;
    SDL_Texture* spawnPlaceTexture;
    SDL_Texture* boxTexture; 
    SDL_Texture* duckTexture;

    //texturas de los items
    SDL_Texture* AK47Texture;
    SDL_Texture* CowboyPistolTexture;
    SDL_Texture* DuelPistolTexture;
    SDL_Texture* LaserRifleTexture;
    SDL_Texture* MagnumTexture;
    SDL_Texture* PewPewLaserTexture;
    SDL_Texture* ShotgunTexture;
    SDL_Texture* SniperTexture;

    SDL_Texture* ArmorTexture;
    SDL_Texture* HelmetTexture;

    SDL_Texture* GrenadaTexture;
    SDL_Texture* BananaTexture;


    std::unordered_map<std::string, Crate> crates;
    std::unordered_map<std::string, SpawnPlace> spawn_places;
    std::unordered_map<std::string, Box> boxes;
    std::unordered_map<std::string, Duck> ducks;
    std::unordered_map<std::string, Item> items;
    

    SDL_Texture* selectedTexture; 


    //Estas son las posiciones donde estan los elementos en la tools area
    int crateX, crateY; 
    int spawnPlaceX, spawnPlaceY;
    int boxX, boxY;
    int duckX, duckY;
   

    enum Tool {
        NONE,
        CREATE_CRATE,
        CREATE_SPAWN_PLACE,
        CREATE_BOX,
        CREATE_DUCK,
        CREATE_AK47,
        CREATE_COWBOY_PISTOL,
        CREATE_DUEL_PISTOL,
        CREATE_LASER_RIFLE,
        CREATE_MAGNUM,
        CREATE_PEWPEW_LASER,
        CREATE_SHOTGUN,
        CREATE_SNIPER,
        CREATE_ARMOR,
        CREATE_HELMET,
        CREATE_GRENADE,
        CREATE_BANANA
    };

    Tool currentTool;
};

#endif // LEVEL_EDITOR_H
