#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>

class GameMap {

private:
    int width;
    int height;
    std::vector<std::vector<int>> map;

public:
    GameMap(int width, int height);

    bool placeDuck(int x, int y);

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

};

#endif
