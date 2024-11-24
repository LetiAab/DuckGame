#include <string>
#include <vector>
#include <memory>
#include "common/constants.h"
#include "common/position.h"
#include <fstream> 

#ifndef PARSER_H
#define PARSER_H

    struct ItemConfig{
        int x;
        int y;
        int item_id;
    };

    struct MapConfig{
        int map_width;
        int map_height;
        std::vector<std::vector<char>> map;
    };

class Parser {
public:
    Parser(const std::string& filePath);

    MapConfig getMap();
    std::vector<Position> get_ducks_positions();
    std::vector<Position> get_spawn_places();
    std::vector<ItemConfig> get_items();

    ~Parser();

private:

    MapConfig map;
    std::vector<Position> ducks_positions;
    std::vector<Position> spawn_places_positions;
    std::vector<ItemConfig> items_positions;

    std::string filePath;
    std::ifstream fileStream;

    bool parserFile();
    void openFile();
    void closeFile();
    std::string readLine();

};

#endif
