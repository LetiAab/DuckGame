#ifndef CLIENT_GAMEMAP_H
#define CLIENT_GAMEMAP_H

#include <vector>

class ClientGameMap {

private:
    int width;
    int height;

public:
    std::vector<std::vector<char>> map;

    ClientGameMap(int width, int height);

    void setMap(const std::vector<std::vector<char>>& new_map);

    int get_height();
    int get_width();

    std::vector<std::vector<char>> getMap();

    char get_position(int x, int y);

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

};

#endif
