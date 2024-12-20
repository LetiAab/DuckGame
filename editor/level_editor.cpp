#include "level_editor.h"

#include <chrono>
#include <SDL_image.h>
#include <iostream>
#include <sstream> 

std::unordered_map<uint8_t, std::string> item_map = {
    {ARMOR_ID, "ARMOR_ID"},
    {HELMET_ID, "HELMET_ID"},
    {BASE_WEAPON_ID, "BASE_WEAPON_ID"},
    {GRENADE_ID, "GRENADE_ID"},
    {BANANA_ID, "BANANA_ID"},
    {PEW_PEW_LASER_ID, "PEW_PEW_LASER_ID"},
    {LASER_RIFLE_ID, "LASER_RIFLE_ID"},
    {AK_47_ID, "AK_47_ID"},
    {DUEL_PISTOL_ID, "DUEL_PISTOL_ID"},
    {COWBOY_PISTOL_ID, "COWBOY_PISTOL_ID"},
    {MAGNUM_ID, "MAGNUM_ID"},
    {SHOTGUN_ID, "SHOTGUN_ID"},
    {SNIPER_ID, "SNIPER_ID"}
};

std::unordered_map<std::string, uint8_t> reverse_item_map = {
    {"ARMOR_ID", ARMOR_ID},
    {"HELMET_ID", HELMET_ID},
    {"BASE_WEAPON_ID", BASE_WEAPON_ID},
    {"GRENADE_ID", GRENADE_ID},
    {"BANANA_ID", BANANA_ID},
    {"PEW_PEW_LASER_ID", PEW_PEW_LASER_ID},
    {"LASER_RIFLE_ID", LASER_RIFLE_ID},
    {"AK_47_ID", AK_47_ID},
    {"DUEL_PISTOL_ID", DUEL_PISTOL_ID},
    {"COWBOY_PISTOL_ID", COWBOY_PISTOL_ID},
    {"MAGNUM_ID", MAGNUM_ID},
    {"SHOTGUN_ID", SHOTGUN_ID},
    {"SNIPER_ID", SNIPER_ID}
};

LevelEditor::LevelEditor()
    : window(nullptr), renderer(nullptr), backgroundTexture(nullptr), crateTexture(nullptr),
      selectedTexture(nullptr), 
      
      crateX(WINDOW_WIDTH + TILE_SIZE), crateY(TILE_SIZE), 
      spawnPlaceX(WINDOW_WIDTH + TILE_SIZE), spawnPlaceY(TILE_SIZE + GRID_CELL_SIZE ), 
      boxX(WINDOW_WIDTH + TILE_SIZE), boxY(TILE_SIZE + 2 * GRID_CELL_SIZE),
      duckX(WINDOW_WIDTH + 4 * GRID_CELL_SIZE), duckY(TILE_SIZE + 4 * GRID_CELL_SIZE),
      
      // Inicialización de las posiciones de los items, con una distancia de 2 celdas
      currentTool(NONE)

{}


LevelEditor::~LevelEditor() {}

std::string generateKey(int x, int y) {
    return std::to_string(x) + "," + std::to_string(y);
}

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

    boxTexture = IMG_LoadTexture(renderer, "../client/imgs/box.png");
    if (!boxTexture) {
        SDL_Log("Failed to load tree texture: %s", SDL_GetError());
        return false;
    }

    duckTexture = IMG_LoadTexture(renderer, "../client/imgs/duck.png");
    if (!duckTexture) {
        SDL_Log("Failed to load tree texture: %s", SDL_GetError());
        return false;
    }

    // Cargando las texturas de los items
    AK47Texture = IMG_LoadTexture(renderer, "../client/imgs/guns/AK-47.png");
    if (!AK47Texture) {
        SDL_Log("Failed to load AK47 texture: %s", SDL_GetError());
        return false;
    }

    CowboyPistolTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/cowboy_pistol.png");
    if (!CowboyPistolTexture) {
        SDL_Log("Failed to load Cowboy Pistol texture: %s", SDL_GetError());
        return false;
    }

    DuelPistolTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/duel_pistol.png");
    if (!DuelPistolTexture) {
        SDL_Log("Failed to load Duel Pistol texture: %s", SDL_GetError());
        return false;
    }

    LaserRifleTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/laser_rifle.png");
    if (!LaserRifleTexture) {
        SDL_Log("Failed to load Laser Rifle texture: %s", SDL_GetError());
        return false;
    }

    MagnumTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/magnum.png");
    if (!MagnumTexture) {
        SDL_Log("Failed to load Magnum texture: %s", SDL_GetError());
        return false;
    }

    PewPewLaserTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/pew_pew_laser.png");
    if (!PewPewLaserTexture) {
        SDL_Log("Failed to load Pew Pew Laser texture: %s", SDL_GetError());
        return false;
    }

    ShotgunTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/shotgun.png");
    if (!ShotgunTexture) {
        SDL_Log("Failed to load Shotgun texture: %s", SDL_GetError());
        return false;
    }

    SniperTexture = IMG_LoadTexture(renderer, "../client/imgs/guns/sniper.png");
    if (!SniperTexture) {
        SDL_Log("Failed to load Sniper texture: %s", SDL_GetError());
        return false;
    }

    ArmorTexture = IMG_LoadTexture(renderer, "../client/imgs/armor/armor.png");
    if (!ArmorTexture) {
        SDL_Log("Failed to load Armor texture: %s", SDL_GetError());
        return false;
    }

    HelmetTexture = IMG_LoadTexture(renderer, "../client/imgs/armor/helmet.png");
    if (!HelmetTexture) {
        SDL_Log("Failed to load Helmet texture: %s", SDL_GetError());
        return false;
    }

    GrenadaTexture = IMG_LoadTexture(renderer, "../client/imgs/grenades/grenade_with_pin.png");
    if (!GrenadaTexture) {
        SDL_Log("Failed to load Grenade texture: %s", SDL_GetError());
        return false;
    }

    BananaTexture = IMG_LoadTexture(renderer, "../client/imgs/bananas/banana.png");
    if (!BananaTexture) {
        SDL_Log("Failed to load Banana texture: %s", SDL_GetError());
        return false;
    }

    selectedTexture = crateTexture; // Textura seleccionada por defecto!! (Esto no anda igual. NO IMPORTA)

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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect toolsArea = {WINDOW_WIDTH, 0, 200, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &toolsArea);

    SDL_Rect crateButtonRect = { crateX, crateY, GRID_CELL_SIZE, GRID_CELL_SIZE };
    SDL_RenderCopy(renderer, crateTexture, nullptr, &crateButtonRect);

    SDL_Rect spawnPlaceRect = { spawnPlaceX, spawnPlaceY, GRID_CELL_SIZE, GRID_CELL_SIZE };
    SDL_RenderCopy(renderer, spawnPlaceTexture, nullptr, &spawnPlaceRect);

    SDL_Rect boxRect = { boxX, boxY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, boxTexture, nullptr, &boxRect);

    SDL_Rect duckRect = { duckX, duckY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 3 };
    SDL_RenderCopy(renderer, duckTexture, nullptr, &duckRect);

    int itemX = crateX;
    int itemY = crateY + GRID_CELL_SIZE * 4;

    // AK47 
    SDL_Rect ak47Rect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, AK47Texture, nullptr, &ak47Rect);

    // Cowboy Pistol
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect cowboyPistolRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, CowboyPistolTexture, nullptr, &cowboyPistolRect);

    // Duel Pistol
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect duelPistolRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, DuelPistolTexture, nullptr, &duelPistolRect);

    // Laser Rifle
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect laserRifleRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, LaserRifleTexture, nullptr, &laserRifleRect);

    // Magnum
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect magnumRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, MagnumTexture, nullptr, &magnumRect);

    // PewPew Laser
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect pewPewLaserRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, PewPewLaserTexture, nullptr, &pewPewLaserRect);

    // Shotgun
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect shotgunRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, ShotgunTexture, nullptr, &shotgunRect);

    // Sniper
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect sniperRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, SniperTexture, nullptr, &sniperRect);

    // Armor
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect armorRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, ArmorTexture, nullptr, &armorRect);

    // Helmet
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect helmetRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, HelmetTexture, nullptr, &helmetRect);

    // Grenade
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect grenadeRect = { itemX, itemY, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
    SDL_RenderCopy(renderer, GrenadaTexture, nullptr, &grenadeRect);

    // Banana
    itemY += GRID_CELL_SIZE * 2;
    SDL_Rect bananaRect = { itemX, itemY, GRID_CELL_SIZE, GRID_CELL_SIZE };
    SDL_RenderCopy(renderer, BananaTexture, nullptr, &bananaRect);



}

void LevelEditor::renderElements(){

    for (auto& pair : crates) {
        Crate& crate = pair.second;
        SDL_Rect crateRect = { crate.x, crate.y, GRID_CELL_SIZE, GRID_CELL_SIZE };  // Asumimos tamaño de 32x32 para cada Crate
        SDL_RenderCopy(renderer, crate.texture, nullptr, &crateRect);
    }

    for (auto& pair : spawn_places) {
        SpawnPlace& spawnPLace = pair.second;
        SDL_Rect spawnPlaceRect = { spawnPLace.x, spawnPLace.y, GRID_CELL_SIZE, GRID_CELL_SIZE };  // Asumimos tamaño de 32x32 para cada Crate
        SDL_RenderCopy(renderer, spawnPLace.texture, nullptr, &spawnPlaceRect);
    }

    for (auto& pair : boxes) {
        Box& box = pair.second;
        SDL_Rect boxRect = { box.x, box.y, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2};  // Asumimos tamaño de 32x32 para cada Crate
        SDL_RenderCopy(renderer, box.texture, nullptr, &boxRect);
    }

    for (auto& pair : ducks) {
        Duck& duck = pair.second;
        SDL_Rect duckRect = { duck.x, duck.y, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 3 };  //  tamaño de 40x64 para cada pato
        SDL_RenderCopy(renderer, duckTexture, nullptr, &duckRect);
    }

    for (auto& pair : items) {
        Item& item = pair.second;
        SDL_Rect itemRect = { item.x, item.y, GRID_CELL_SIZE * 2, GRID_CELL_SIZE * 2 };
        SDL_RenderCopy(renderer, item.texture, nullptr, &itemRect);
    }
    


}

void LevelEditor::handleEvent(SDL_Event& event, bool& running) {
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_s) {
                saveLevel(); 
            }

            if(event.key.keysym.sym == SDLK_u){
                uploadLevel("../editor/levels/level.txt");
            }
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


                if (mouseX >= boxX && mouseX <= boxX + GRID_CELL_SIZE &&
                    mouseY >= boxY && mouseY <= boxY + GRID_CELL_SIZE) {
                    selectedTexture = boxTexture;
                    currentTool = CREATE_BOX;
                }


                if (mouseX >= duckX && mouseX <= duckX + GRID_CELL_SIZE * 2 &&
                    mouseY >= duckY && mouseY <= duckY + GRID_CELL_SIZE * 3) {
                    selectedTexture = boxTexture;
                    currentTool = CREATE_DUCK;
                }

            // AK47
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 4 && mouseY <= crateY + GRID_CELL_SIZE * 6) {
                    selectedTexture = AK47Texture;
                    currentTool = CREATE_AK47;
                }

                // Cowboy Pistol
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 6 && mouseY <= crateY + GRID_CELL_SIZE * 8) {
                    selectedTexture = CowboyPistolTexture;
                    currentTool = CREATE_COWBOY_PISTOL;
                }

                // Duel Pistol
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 8 && mouseY <= crateY + GRID_CELL_SIZE * 10) {
                    selectedTexture = DuelPistolTexture;
                    currentTool = CREATE_DUEL_PISTOL;
                }

                // Laser Rifle
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 10 && mouseY <= crateY + GRID_CELL_SIZE * 12) {
                    selectedTexture = LaserRifleTexture;
                    currentTool = CREATE_LASER_RIFLE;
                }

                // Magnum
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 12 && mouseY <= crateY + GRID_CELL_SIZE * 14) {
                    selectedTexture = MagnumTexture;
                    currentTool = CREATE_MAGNUM;
                }

                // PewPew Laser
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 14 && mouseY <= crateY + GRID_CELL_SIZE * 16) {
                    selectedTexture = PewPewLaserTexture;
                    currentTool = CREATE_PEWPEW_LASER;
                }

                // Shotgun
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 16 && mouseY <= crateY + GRID_CELL_SIZE * 18) {
                    selectedTexture = ShotgunTexture;
                    currentTool = CREATE_SHOTGUN;
                }

                // Sniper
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 18 && mouseY <= crateY + GRID_CELL_SIZE * 20) {
                    selectedTexture = SniperTexture;
                    currentTool = CREATE_SNIPER;
                }

                // Armor
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 20 && mouseY <= crateY + GRID_CELL_SIZE * 22) {
                    selectedTexture = ArmorTexture;
                    currentTool = CREATE_ARMOR;
                }

                // Helmet
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 22 && mouseY <= crateY + GRID_CELL_SIZE * 24) {
                    selectedTexture = HelmetTexture;
                    currentTool = CREATE_HELMET;
                }

                // Grenade
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE * 2 &&
                    mouseY >= crateY + GRID_CELL_SIZE * 24 && mouseY <= crateY + GRID_CELL_SIZE * 26) {
                    selectedTexture = GrenadaTexture;
                    currentTool = CREATE_GRENADE;
                }

                // Banana
                if (mouseX >= crateX && mouseX <= crateX + GRID_CELL_SIZE &&
                    mouseY >= crateY + GRID_CELL_SIZE * 26 && mouseY <= crateY + GRID_CELL_SIZE * 28) {
                    selectedTexture = BananaTexture;
                    currentTool = CREATE_BANANA;
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
                    
                    crates[generateKey(roundedX, roundedY)] = newCrate;
                    occupancyGrid[gridX][gridY] = true;
                }

                if (currentTool == CREATE_SPAWN_PLACE && mouseX < WINDOW_WIDTH) {

                    SpawnPlace newSpawnPlace = { roundedX, roundedY, selectedTexture };
                    spawn_places[generateKey(roundedX, roundedY)] = newSpawnPlace;
                    occupancyGrid[gridX][gridY] = true;
                }

                if (currentTool == CREATE_BOX && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Box newBox = { roundedX, roundedY, selectedTexture };
                    boxes[generateKey(roundedX, roundedY)] = newBox;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;

                }

                if (currentTool == CREATE_DUCK && mouseX < WINDOW_WIDTH) {

                    if (ducks.size() > 6) {
                        break;
                    }

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true ||
                        occupancyGrid[gridX][gridY + 2] == true ||
                        occupancyGrid[gridX + 1][gridY + 2] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }
                        

                    Duck newDuck = { roundedX, roundedY, selectedTexture };
                    ducks[generateKey(roundedX, roundedY)] = newDuck;
                    occupancyGrid[gridX][gridY] = true ;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true ;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                    occupancyGrid[gridX][gridY + 2] = true ;
                    occupancyGrid[gridX + 1][gridY + 2] = true;         
                }

                if (currentTool == CREATE_AK47 && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, AK_47_ID, AK47Texture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_COWBOY_PISTOL && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, COWBOY_PISTOL_ID, CowboyPistolTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_DUEL_PISTOL && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, DUEL_PISTOL_ID, DuelPistolTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_LASER_RIFLE && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, LASER_RIFLE_ID ,LaserRifleTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;

                }

                if (currentTool == CREATE_MAGNUM && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, MAGNUM_ID, MagnumTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_PEWPEW_LASER && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, PEW_PEW_LASER_ID ,PewPewLaserTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_SHOTGUN && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, SHOTGUN_ID, ShotgunTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_SNIPER && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, SNIPER_ID ,SniperTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_ARMOR && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, ARMOR_ID ,ArmorTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_HELMET && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, HELMET_ID, HelmetTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_GRENADE && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, GRENADE_ID, GrenadaTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                if (currentTool == CREATE_BANANA && mouseX < WINDOW_WIDTH) {

                    if (occupancyGrid[gridX][gridY] == true ||
                        occupancyGrid[gridX + 1][gridY] == true ||
                        occupancyGrid[gridX][gridY + 1] == true ||
                        occupancyGrid[gridX + 1][gridY + 1] == true
                    ){
                        std::cout << "ESTA OCUPADO !!" << std::endl;
                        break;
                    }

                    Item newItem = { roundedX, roundedY, BANANA_ID, BananaTexture };
                    items[generateKey(roundedX, roundedY)] = newItem;
                    occupancyGrid[gridX][gridY] = true;
                    occupancyGrid[gridX + 1][gridY] = true;
                    occupancyGrid[gridX][gridY + 1] = true;
                    occupancyGrid[gridX + 1][gridY + 1] = true;
                }

                                


            } 
            
            if (event.button.button == SDL_BUTTON_RIGHT) {
                std::cout << "CLICK DERECHO: " << std::endl;

                int mouseX = event.button.x;
                int mouseY = event.button.y;

                int roundedX = (mouseX / (GRID_CELL_SIZE)) * (GRID_CELL_SIZE); 
                int roundedY = (mouseY / (GRID_CELL_SIZE)) * (GRID_CELL_SIZE);

                int gridX = roundedX / GRID_CELL_SIZE;
                int gridY = roundedY / GRID_CELL_SIZE;


                 std::string key = generateKey(roundedX, roundedY);

                deleteElement(key, gridX, gridY);

            }
            break;
    }
}

void LevelEditor::deleteElement(std::string key, int gridX, int gridY){

    if (crates.find(key) != crates.end()) {
        crates.erase(key);
        occupancyGrid[gridX][gridY] = false;        
    } 
    if (spawn_places.find(key) != spawn_places.end()) {
        spawn_places.erase(key);
        occupancyGrid[gridX][gridY] = false;        
    } 
    if (boxes.find(key) != boxes.end()) {
        boxes.erase(key);
        occupancyGrid[gridX][gridY] = false;
        occupancyGrid[gridX + 1][gridY] = false;
        occupancyGrid[gridX][gridY + 1] = false;
        occupancyGrid[gridX + 1][gridY + 1] = false;
    } 
    if (ducks.find(key) != ducks.end()) {
        ducks.erase(key);
        occupancyGrid[gridX][gridY] = false ;
        occupancyGrid[gridX + 1][gridY] = false;
        occupancyGrid[gridX][gridY + 1] = false ;
        occupancyGrid[gridX + 1][gridY + 1] = false;
        occupancyGrid[gridX][gridY + 2] = false;
        occupancyGrid[gridX + 1][gridY + 2] = false;
    } 

    if (items.find(key) != items.end()) {
        items.erase(key);
        if (boxes.find(key) != boxes.end()) {
        boxes.erase(key);
        occupancyGrid[gridX][gridY] = false;
        occupancyGrid[gridX + 1][gridY] = false;
        occupancyGrid[gridX][gridY + 1] = false;
        occupancyGrid[gridX + 1][gridY + 1] = false;
        } 
    } 

}

// Modificar el bucle de renderizado para llamar a drawDottedGrid
void LevelEditor::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        const auto start = std::chrono::high_resolution_clock::now();

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

        const auto end = std::chrono::high_resolution_clock::now();
        auto loop_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        auto sleep_duration = DELAY_TIME - loop_duration;

        if (sleep_duration > 0) {
            SDL_Delay(sleep_duration);
        }
    }
}


void LevelEditor::saveLevel() {
    std::ofstream levelFile("../editor/levels/level.txt");
    if (!levelFile.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo para guardar el nivel." << std::endl;
        return;
    }
    levelFile << MATRIX_N << "\n";
    levelFile << MATRIX_M << "\n";
    levelFile << "\n";

    levelFile << "CRATES\n";
    for (const auto& [key, crate] : crates) {
        levelFile << std::dec << crate.x / 4 << "," << crate.y / 4 << "\n";
    }
    levelFile << "\n";

    levelFile << "SPAWN PLACE\n";
    for (const auto& [key, spawnPlace] : spawn_places) {
        levelFile << std::dec << spawnPlace.x / 4 << "," << spawnPlace.y / 4 << "\n";
    }
    levelFile << "\n";

    levelFile << "BOX\n";
    for (const auto& [key, box] : boxes) {
        levelFile << std::dec << box.x / 4 << "," << box.y / 4 << "\n";
    }
    levelFile << "\n";

    levelFile << "ITEM\n";
    for (const auto& [key, item] : items) {
        levelFile << std::dec 
                  << item.x / 4 << "," << item.y / 4 << ","
                  << item_map[item.item_id] << "\n";
    }
    levelFile << "\n";

    levelFile << "SPAWN DUCK\n";
    for (const auto& [key, duck] : ducks) {
        levelFile << std::dec << duck.x / 4 << "," << duck.y / 4 << "\n";
    }
    levelFile << "\n";

    levelFile.close();
    std::cout << "Nivel guardado exitosamente en level.txt" << std::endl;
}



// -----------------------UPLOAD--------------------------------------------

bool parseCoordinates(const std::string& line, int& x, int& y) {
    std::istringstream iss(line);
    std::string token;
    if (std::getline(iss, token, ',')) x = std::stoi(token);
    if (std::getline(iss, token, ',')) y = std::stoi(token);
    return true;
}

bool parseItem(const std::string& line, int& x, int& y, std::string& itemId) {
    std::istringstream iss(line);
    std::string token;
    if (std::getline(iss, token, ',')) x = std::stoi(token);
    if (std::getline(iss, token, ',')) y = std::stoi(token);
    if (std::getline(iss, token, ',')) itemId = token;
    return true;
}

SDL_Texture* LevelEditor::getItemTexture(uint8_t itemId){
    switch(itemId){

        case(AK_47_ID):
            return AK47Texture;
        case(COWBOY_PISTOL_ID):
            return CowboyPistolTexture;
        case(DUEL_PISTOL_ID):
            return DuelPistolTexture;
        case (LASER_RIFLE_ID):
            return LaserRifleTexture;
        case(MAGNUM_ID):
            return MagnumTexture;
        case (PEW_PEW_LASER_ID):
            return PewPewLaserTexture;
        case (SHOTGUN_ID):
            return ShotgunTexture;
        case (SNIPER_ID):
            return SniperTexture;
        case (ARMOR_ID):
            return ArmorTexture;
        case (HELMET_ID):
            return HelmetTexture;
        
        case (GRENADE_ID):
            return GrenadaTexture;
        
        default:
            return BananaTexture;
        


    }
}

void LevelEditor::uploadLevel(const std::string& filePath) {
    std::ifstream levelFile(filePath);
    if (!levelFile.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo para cargar el nivel." << std::endl;
        return;
    }

    // Limpiar estructuras existentes antes de cargar
    crates.clear();
    spawn_places.clear();
    boxes.clear();
    items.clear();
    ducks.clear();

    std::string line;
    int matrix_n = 0, matrix_m = 0;

    // Leer dimensiones de la matriz
    if (std::getline(levelFile, line)) matrix_n = std::stoi(line);
    if (std::getline(levelFile, line)) matrix_m = std::stoi(line);

    std::cout << "MAPA DE " << matrix_n << " X " << matrix_m; 

    // Leer secciones
    while (std::getline(levelFile, line)) {
        if (line.empty()) continue; // Ignorar líneas vacías

        if (line == "CRATES") {
            while (std::getline(levelFile, line) && !line.empty()) {
                int x, y;
                if (parseCoordinates(line, x, y)) {
                    crates[generateKey(x * TILE_SIZE, y * TILE_SIZE)] = Crate{x * TILE_SIZE, y * TILE_SIZE, crateTexture};
                }
            }
        } else if (line == "SPAWN PLACE") {
            while (std::getline(levelFile, line) && !line.empty()) {
                int x, y;
                if (parseCoordinates(line, x, y)) {
                    spawn_places[generateKey(x * TILE_SIZE, y * TILE_SIZE)] = SpawnPlace{x * TILE_SIZE, y * TILE_SIZE, spawnPlaceTexture};
                }
            }
        } else if (line == "BOX") {
            while (std::getline(levelFile, line) && !line.empty()) {
                int x, y;
                if (parseCoordinates(line, x, y)) {
                    boxes[generateKey(x * TILE_SIZE,y * TILE_SIZE)] =  Box{x * TILE_SIZE, y * TILE_SIZE, boxTexture};
                }
            }
        } else if (line == "ITEM") {
            while (std::getline(levelFile, line) && !line.empty()) {
                int x, y;
                std::string itemId;
                if (parseItem(line, x, y, itemId)) {
                    items[generateKey(x*TILE_SIZE, y*TILE_SIZE)] = Item{x * TILE_SIZE, y * TILE_SIZE, reverse_item_map[itemId], getItemTexture(reverse_item_map[itemId])};
                }
            }
        } else if (line == "SPAWN DUCK") {
            while (std::getline(levelFile, line) && !line.empty()) {
                int x, y;
                if (parseCoordinates(line, x, y)) {
                    ducks[generateKey(x*TILE_SIZE, y*TILE_SIZE)] =  Duck{x * 4, y * 4, duckTexture};
                }
            }
        }
    }

    levelFile.close();
    std::cout << "Nivel cargado exitosamente desde " << filePath << std::endl;
}





void LevelEditor::cleanup() {
    //ver si hace falta borrar las texturas de los items
    if (crateTexture) SDL_DestroyTexture(crateTexture);
    if (spawnPlaceTexture) SDL_DestroyTexture(spawnPlaceTexture);
    if (boxTexture) SDL_DestroyTexture(boxTexture);
    if (duckTexture) SDL_DestroyTexture(duckTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}



