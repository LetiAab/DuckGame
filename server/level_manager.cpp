#include "level_manager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>

const char PLATFORM = 'P';
const char BOX = 'B';
const char EMPTY = ' ';

std::unordered_map<std::string, uint8_t> item_map = {
    {"ARMOR_ID", ARMOR_ID},
    {"HELMET_ID", HELMET_ID},
    {"BASE_WEAPON_ID", BASE_WEAPON_ID},
    {"GRANADA_ID", GRANADA_ID},
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

LevelManager::LevelManager() : filePath("../editor/levels/level.txt"), counter(0) {
    //cargo los niveles
    //si los archivos no son modificables, deberia poder leer los que estan disponibles
    level_paths.emplace_back("../editor/levels/level.txt");
    level_paths.emplace_back("../editor/levels/level1.txt");
    level_paths.emplace_back("../editor/levels/level2.txt");
}

LevelManager::~LevelManager() {
    closeFile();
}

void LevelManager::choose_level(){
    clear_old_level();

    if(counter == static_cast<int>(level_paths.size())) {
        counter = 0;
    }
    //los voy intercalando, pero podria ser random
    filePath = level_paths[counter];
    counter++;
    
    
    parserFile();
}

void LevelManager::clear_old_level(){
    map.map.clear();
    ducks_positions.clear();
    spawn_places_positions.clear();
    items_positions.clear();
    boxes_positions.clear();
}

void LevelManager::openFile() {
    fileStream.open(filePath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
}

void LevelManager::closeFile() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

std::string LevelManager::readLine() {
    std::string line;
    if (std::getline(fileStream, line)) {
        return line;
    }
    throw std::runtime_error("Error al leer línea del archivo: " + filePath);
}

bool LevelManager::parserFile() {
    openFile();

    try {
        map.map_height = std::stoi(readLine());
        map.map_width = std::stoi(readLine());
        map.map = std::vector<std::vector<char>>(map.map_height, std::vector<char>(map.map_width, EMPTY));

        std::string line, section;
        while (fileStream.good()) {
            line = readLine();
            if (line.empty()) continue;

            std::cout << "Processing line: " << line << std::endl;

            if (line == "CRATES" || line == "SPAWN PLACE" || line == "BOX" ||
                line == "ITEM" || line == "SPAWN DUCK") {
                section = line;
                std::cout << "Section found: " << section << std::endl;
                continue;
            }

            std::istringstream ss(line);
            int x, y;
            uint8_t item_id;

            if (section == "CRATES") {
                if (ss >> x && ss.ignore() && ss >> y) {
                    std::cout << "Placed CRATE at (" << x << ", " << y << ")" << std::endl;

                    for (int i = x; i < x + PLATFORM_SIZE_X; ++i) {
                        for (int j = y; j < y + PLATFORM_SIZE_Y; ++j) {
                            if (i < map.map_width && j < map.map_height) {
                                map.map[j][i] = PLATFORM;
                            }
                        }
                    }
                } else {
                    std::cerr << "Error parsing CRATES at line: " << line << std::endl;
                }
            } else if (section == "SPAWN PLACE") {
                if (ss >> x && ss.ignore() && ss >> y) {
                    spawn_places_positions.emplace_back(x, y);
                    std::cout << "Spawn place at (" << x << ", " << y << ")" << std::endl;
                } else {
                    std::cerr << "Error parsing SPAWN PLACE at line: " << line << std::endl;
                }

            } else if (section == "BOX") {
                if (ss >> x && ss.ignore() && ss >> y) {
                    boxes_positions.emplace_back(x, y);
                    std::cout << "Placed BOX at (" << x << ", " << y << ")" << std::endl;

                    for (int i = x; i < x + BOX_SIZE_X; ++i) {
                        for (int j = y; j < y + BOX_SIZE_Y; ++j) {
                            if (i < map.map_width && j < map.map_height) {
                                map.map[j][i] = BOX;
                            }
                        }
                    }

                } else {
                    std::cerr << "Error parsing BOX at line: " << line << std::endl;
                }

            } else if (section == "ITEM") {
                std::string item_name;  

                if (ss >> x && ss.ignore() && ss >> y && ss.ignore() && ss >> item_name) {
                    auto it = item_map.find(item_name);
                    if (it != item_map.end()) {
                        item_id = it->second; 
                        items_positions.push_back({x, y, item_id});
                        std::cout << "Item " << item_name << " (ID: " << static_cast<int>(item_id) << ") placed at (" << x << ", " << y << ")" << std::endl;
                    } else {
                        std::cerr << "Error: Item name not found in map: " << item_name << std::endl;
                    }
                } else {
                    std::cerr << "Error parsing ITEM at line: " << line << std::endl;
                }


            } else if (section == "SPAWN DUCK") {
                if (ss >> x && ss.ignore() && ss >> y) {
                    ducks_positions.emplace_back(x, y);
                    std::cout << "Duck spawned at (" << x << ", " << y << ")" << std::endl;

                    int duck_id = ducks_positions.size();
                    for (int i = x; i < x + DUCK_SIZE_X; ++i) {
                        for (int j = y; j < y + DUCK_SIZE_Y; ++j) {
                            if (i < map.map_width && j < map.map_height) {
                                map.map[j][i] = '0' + duck_id;
                                //std::cout << "Placed duck " << duck_id << " at (" << i << ", " << j << ")" << std::endl;
                            }
                        }
                    }
                } else {
                    std::cerr << "Error parsing SPAWN DUCK at line: " << line << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        //std::cerr << "Exception during parsing: " << e.what() << std::endl;
        closeFile();
        return true;
    }

    closeFile();
    return true;
}


MapConfig LevelManager::getMap() {
    return map;
}

std::vector<Position> LevelManager::get_ducks_positions() {
    return ducks_positions;
}

std::vector<Position> LevelManager::get_spawn_places() {
    return spawn_places_positions;
}

std::vector<Position> LevelManager::get_boxes() {
    return boxes_positions;
}

std::vector<ItemConfig> LevelManager::get_items() {
    return items_positions;
}


