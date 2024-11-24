#include "level_editor.h"
#include <SDL_image.h>
#include <iostream>

LevelEditor::LevelEditor()
    : window(nullptr), renderer(nullptr), backgroundTexture(nullptr), crateTexture(nullptr),
      selectedTexture(nullptr), crateX(WINDOW_WIDTH + TILE_SIZE), crateY(TILE_SIZE), selectedCrateIndex(0), currentTool(NONE) {}

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

    selectedTexture = crateTexture; // Default selected texture (crate)

    return true;
}

// Función para dibujar la cuadrícula punteada
void LevelEditor::drawDottedGrid() {
    // Definir el color de la línea punteada (por ejemplo, gris claro)
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // Gris claro

    // Establecer la longitud del segmento de línea (en píxeles)
    const int dashLength = 5;  // Longitud de cada segmento de la línea punteada
    const int spaceLength = 3; // Longitud del espacio entre segmentos

    // Dibujar las líneas verticales
    for (int x = 0; x < WINDOW_WIDTH; x += TILE_SIZE * TILE_SIZE) {
        for (int i = 0; i < WINDOW_HEIGHT; i += dashLength + spaceLength) {
            // Dibujar el segmento de la línea
            SDL_RenderDrawLine(renderer, x, i, x, i + dashLength);
        }
    }

    // Dibujar las líneas horizontales
    for (int y = 0; y < WINDOW_HEIGHT; y += TILE_SIZE * TILE_SIZE) {
        for (int i = 0; i < WINDOW_WIDTH; i += dashLength + spaceLength) {
            // Dibujar el segmento de la línea
            SDL_RenderDrawLine(renderer, i, y, i + dashLength, y);
        }
    }
}

// Modificar el bucle de renderizado para llamar a drawDottedGrid
void LevelEditor::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        // Si se hace clic en la zona de herramientas, cambiar el sprite seleccionado
                        if (mouseX >= crateX && mouseX <= crateX + TILE_SIZE * TILE_SIZE &&
                            mouseY >= crateY && mouseY <= crateY + TILE_SIZE * TILE_SIZE) {
                            selectedTexture = crateTexture; // Seleccionar el crate
                            currentTool = CREATE_CRATE; // Activar la herramienta para crear crates
                        }

                        // Si se hace clic en el mapa, colocar el crate seleccionado
                        if (currentTool == CREATE_CRATE && mouseX < WINDOW_WIDTH) {
                            // Redondear la posición de clic al múltiplo más cercano de TILE_SIZE


                            //Me vueven loco las matematicas tiooo                            
                            int roundedX = std::floor((mouseX / (TILE_SIZE * TILE_SIZE))) * (TILE_SIZE * TILE_SIZE); 
                            int roundedY = std::floor((mouseY / (TILE_SIZE * TILE_SIZE))) * (TILE_SIZE * TILE_SIZE);

                            // Asegurarse de que las coordenadas estén dentro de los límites del mapa
                            if (roundedX >= 0 && roundedX < WINDOW_WIDTH &&
                                roundedY >= 0 && roundedY < WINDOW_HEIGHT) {
                                Crate newCrate = { roundedX, roundedY, selectedTexture };
                                crates.push_back(newCrate);
                            }
                        }
                    }
                    break;
            }
        }

        // Limpiar pantalla
        SDL_RenderClear(renderer);

        // Renderizar el fondo
        SDL_Rect backgroundRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);

        // Renderizar la cuadrícula punteada
        drawDottedGrid();

        // Renderizar el área negra para herramientas
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect toolsArea = {WINDOW_WIDTH, 0, 200, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &toolsArea);

        // Renderizar el botón en la zona de herramientas (el sprite seleccionado)
        SDL_Rect crateButtonRect = { crateX, crateY, TILE_SIZE * TILE_SIZE, TILE_SIZE * TILE_SIZE };
        SDL_RenderCopy(renderer, selectedTexture, nullptr, &crateButtonRect);

        // Renderizar todos los crates en el mapa
        for (auto& crate : crates) {
            SDL_Rect crateRect = { crate.x, crate.y, TILE_SIZE * TILE_SIZE, TILE_SIZE * TILE_SIZE };
            SDL_RenderCopy(renderer, crate.texture, nullptr, &crateRect);
        }

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
