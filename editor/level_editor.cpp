#include "level_editor.h"
#include <SDL_image.h>
#include <iostream>

LevelEditor::LevelEditor()
    : window(nullptr), renderer(nullptr), backgroundTexture(nullptr), crateTexture(nullptr),
      selectedTexture(nullptr), crateX(WINDOW_WIDTH + TILE_SIZE), crateY(TILE_SIZE), selectedCrateIndex(0), 
      spawnPlaceX(WINDOW_WIDTH + TILE_SIZE), spawnPlaceY(TILE_SIZE + std::pow(TILE_SIZE, 2) ), selectedSpawnPlaceIndex(0), currentTool(NONE) {}

LevelEditor::~LevelEditor() {}

bool LevelEditor::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH + 200, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return loadTextures();
}

bool LevelEditor::loadTextures(){

    backgroundTexture = IMG_LoadTexture(renderer, "../client/imgs/backgrounds/forest.png");
    if (!backgroundTexture) {
        SDL_Log("Failed to load background: %s", SDL_GetError());
        return false;
    }

    crateTexture = IMG_LoadTexture(renderer, "../client/imgs/crate.png");
    if (!crateTexture) {
        SDL_Log("Failed to load crate texture: %s", SDL_GetError());
        return false;
    }

    spawnPlaceTexture = IMG_LoadTexture(renderer, "../client/imgs/spawn-place.png");
    if (!spawnPlaceTexture) {
        SDL_Log("Failed to load tree texture: %s", SDL_GetError());
        return false;
    }

    selectedTexture = crateTexture; // Textura seleccionada por defecto!!

    return true;
}

void LevelEditor::drawDottedGrid() {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 

    const int dashLength = 5;  
    const int spaceLength = 3;

    for (int x = 0; x < WINDOW_WIDTH; x += GRID_CELL_SIZE) {
        for (int i = 0; i < WINDOW_HEIGHT; i += dashLength + spaceLength) {
            SDL_RenderDrawLine(renderer, x, i, x, i + dashLength);
        }
    }

    for (int y = 0; y < WINDOW_HEIGHT; y += GRID_CELL_SIZE) {
        for (int i = 0; i < WINDOW_WIDTH; i += dashLength + spaceLength) {
            SDL_RenderDrawLine(renderer, i, y, i + dashLength, y);
        }
    }
}

void LevelEditor::renderToolArea(){

    // Renderizar el área negra para herramientas
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect toolsArea = {WINDOW_WIDTH, 0, 200, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &toolsArea);

    // Renderizar el botón en la zona de herramientas (el sprite seleccionado)
    SDL_Rect crateButtonRect = { crateX, crateY, GRID_CELL_SIZE, GRID_CELL_SIZE };
    SDL_RenderCopy(renderer, crateTexture, nullptr, &crateButtonRect);

    // Renderizar el botón en la zona de herramientas (el sprite seleccionado)
    SDL_Rect spawnPlaceRect = { spawnPlaceX, spawnPlaceY, GRID_CELL_SIZE, GRID_CELL_SIZE };
    SDL_RenderCopy(renderer, spawnPlaceTexture, nullptr, &spawnPlaceRect);


}

void LevelEditor::renderElements(){

    for (auto& crate : crates) {
        SDL_Rect crateRect = { crate.x, crate.y, GRID_CELL_SIZE, GRID_CELL_SIZE };
        SDL_RenderCopy(renderer, crate.texture, nullptr, &crateRect);
    }

    for (auto& spawn_place : spawn_places) {
        SDL_Rect spawnPlaceRect = { spawn_place.x, spawn_place.y, GRID_CELL_SIZE, GRID_CELL_SIZE };
        SDL_RenderCopy(renderer, spawn_place.texture, nullptr, &spawnPlaceRect);
    }

}

void LevelEditor::handleEvent(SDL_Event& event, bool& running) {
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;


                //Si se hace click en el tool area
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE &&
                    mouseY >= crateY && mouseY <= crateY + GRID_CELL_SIZE) {
                    selectedTexture = crateTexture;
                    currentTool = CREATE_CRATE;
                }

                if (mouseX >= spawnPlaceX && mouseX <= spawnPlaceX + GRID_CELL_SIZE &&
                    mouseY >= spawnPlaceY && mouseY <= spawnPlaceY + GRID_CELL_SIZE) {
                    selectedTexture = spawnPlaceTexture;
                    currentTool = CREATE_SPAWN_PLACE;
                }

                // Si se hace clic en el mapa
                int roundedX = (mouseX / (GRID_CELL_SIZE)) * (GRID_CELL_SIZE); 
                int roundedY = (mouseY / (GRID_CELL_SIZE)) * (GRID_CELL_SIZE);

                std::cout << "QUIERO AGREGAR ALGO EN X: " << roundedX << " Y: " << roundedY << std::endl;




                //me dijo si clickeo en la zona del mapa sino break
                if (!(roundedX >= 0 && roundedX < WINDOW_WIDTH &&
                        roundedY >= 0 && roundedY < WINDOW_HEIGHT)) {
                        break;
                }
                    
                int gridX = roundedX / GRID_CELL_SIZE;
                int gridY = roundedY / GRID_CELL_SIZE;

                std::cout << "REVISO GRID PARA AGREGAR ALGO EN X: " << gridX << " Y: " << gridY << std::endl;

                
                //si clickeo en una celda ocupada no me deja
                if (occupancyGrid[gridX][gridY] == true){
                    std::cout << "ESTA OCUPADO !!" << std::endl;
                    break;
                }


                if (currentTool == CREATE_CRATE && mouseX < WINDOW_WIDTH) {

                    Crate newCrate = { roundedX, roundedY, selectedTexture };
                    crates.push_back(newCrate);
                    occupancyGrid[gridX][gridY] = true;
                }

                if (currentTool == CREATE_SPAWN_PLACE && mouseX < WINDOW_WIDTH) {

                    SpawnPlace newSpawnPlace = { roundedX, roundedY, selectedTexture };
                    spawn_places.push_back(newSpawnPlace);
                    occupancyGrid[gridX][gridY] = true;

                }
            }
            break;
    }
}

// Modificar el bucle de renderizado para llamar a drawDottedGrid
void LevelEditor::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            handleEvent(event, running);
        }

        // Limpiar pantalla
        SDL_RenderClear(renderer);

        // Renderizar el fondo
        SDL_Rect backgroundRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);


        //Renderizo cuadricula
        drawDottedGrid();

        //Herramientas
        renderToolArea();   

        //Elementos que ya puse en el mapa
        renderElements();


        // Actualizar pantalla
        SDL_RenderPresent(renderer);
    }
}


void LevelEditor::cleanup() {
    if (crateTexture) SDL_DestroyTexture(crateTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
