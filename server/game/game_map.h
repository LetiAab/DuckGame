#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>

class GameMap {

private:
    int width;
    int height;
    std::vector<std::vector<char>> map;

public:
    GameMap(int width, int height);

    bool placeDuck(int x, int y);

    void setEscenario();

    bool canMoveDuckTo(int x, int y);

    void cleanDuckOldPosition(int x, int y);

    void setDuckNewPosition(int x, int y);

    int get_height();
    int get_width();

    std::vector<std::vector<char>> getMap();

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

};

#endif
