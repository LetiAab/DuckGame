#include "parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>

const char PLATFORM = 'P';
const char BOX = 'B';
const char EMPTY = ' ';

Parser::Parser(const std::string& filePath)
    : filePath(filePath) {
    if (!parserFile()) {
        throw std::runtime_error("Error al parsear el archivo: " + filePath);
    }
}

Parser::~Parser() {
    closeFile();
}

void Parser::openFile() {
    fileStream.open(filePath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
}

void Parser::closeFile() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

std::string Parser::readLine() {
    std::string line;
    if (std::getline(fileStream, line)) {
        return line;
    }
    throw std::runtime_error("Error al leer línea del archivo: " + filePath);
}

bool Parser::parserFile() {
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
                line == "ITEMS" || line == "SPAWN DUCK") {
                section = line;
                std::cout << "Section found: " << section << std::endl;
                continue;
            }

            std::istringstream ss(line);
            int x, y;
            uint8_t item_id;

            if (section == "CRATES") {
                if (ss >> x && ss.ignore() && ss >> y) {
                    map.map[y][x] = PLATFORM;
                    std::cout << "Placed CRATE at (" << x << ", " << y << ")" << std::endl;
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
                    map.map[y][x] = BOX;
                    std::cout << "Placed BOX at (" << x << ", " << y << ")" << std::endl;
                } else {
                    std::cerr << "Error parsing BOX at line: " << line << std::endl;
                }
            } else if (section == "ITEMS") {
                if (ss >> x && ss.ignore() && ss >> y && ss.ignore() && ss >> item_id) {
                    items_positions.push_back({x, y, item_id});
                    std::cout << "Item " << item_id << " placed at (" << x << ", " << y << ")" << std::endl;
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
                                std::cout << "Placed duck " << duck_id << " at (" << i << ", " << j << ")" << std::endl;
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


MapConfig Parser::getMap() {
    return map;
}

std::vector<Position> Parser::get_ducks_positions() {
    return ducks_positions;
}

std::vector<Position> Parser::get_spawn_places() {
    return spawn_places_positions;
}

std::vector<ItemConfig> Parser::get_items() {
    return items_positions;
}


