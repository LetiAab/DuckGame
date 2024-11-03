#ifndef CLIENT_GAMEMAP_H
#define CLIENT_GAMEMAP_H

#include <vector>

class ClientGameMap {

//ESTA CLASE LA COPIE DEL CLIENT MAP DEL SERVER
//TODO: VER QUE COSAS NO SIRVEN


private:
    int width;
    int height;
    std::vector<std::vector<int>> map;

public:
    ClientGameMap(int width, int height);

    bool placeDuck(int x, int y);

    void setEscenario();

    bool canMoveDuckTo(int x, int y);

    void cleanDuckOldPosition(int x, int y);

    void setDuckNewPosition(int x, int y);

    int get_height();
    int get_width();

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

};

#endif
