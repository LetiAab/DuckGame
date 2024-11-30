#include <string>
#include <vector>
#include <memory>
#include "common/constants.h"
#include "common/position.h"
#include <fstream> 
#include <unordered_map>

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

    struct ItemConfig{
        int x;
        int y;
        uint8_t item_id;
    };

    struct MapConfig{
        int map_width;
        int map_height;
        std::vector<std::vector<char>> map;
    };

class LevelManager {
public:
    LevelManager();

    MapConfig getMap();
    std::vector<Position> get_ducks_positions();
    std::vector<Position> get_spawn_places();
    std::vector<Position> get_boxes();
    std::vector<ItemConfig> get_items();

    void choose_level();

    ~LevelManager();

private:

    MapConfig map;
    std::vector<Position> ducks_positions;
    std::vector<Position> spawn_places_positions;
    std::vector<Position> boxes_positions;
    std::vector<ItemConfig> items_positions;

    std::string filePath;
    std::ifstream fileStream;
    std::vector<std::string> level_paths;
    int counter;

    void openFile();
    void closeFile();
    std::string readLine();
    void clear_old_level();
    bool parserFile();

};

#endif
