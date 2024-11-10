#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>

#include "duck.h"

class GameMap {

private:
    int width;
    int height;
    std::vector<std::vector<char>> map;

public:
    GameMap(int width, int height);

    bool placeDuck(int x, int y, char duck_id);

    bool is_duck_touching_floor(int x, int y);

    bool is_element_touching_floor(int x, int y, int size_x, int size_y);

    char duck_in_position(int x, int y, int size_x, int size_y);

    void setEscenario();

    bool canMoveDuckTo(int x, int y, char duck_id);

    void cleanDuckOldPosition(int x, int y);

    void setDuckNewPosition(int x, int y, char duck_id);


    bool canMoveBulletTo(int x, int y);
    void setBulletNewPosition(int x, int y);
    void cleanBulletOldPosition(int x, int y);



    bool can_move_projectile(int x, int y, int size_x, int size_y);

    int projectile_hits_duck(int x, int y);

    void move_projectile(int position_x, int position_y, int speed_x, int speed_y, int size_x, int size_y);

    void clean_projectile_old_position(int x, int y, int size_x, int size_y);

    void set_projectile_new_position(int x, int y,  int size_x, int size_y);

    int get_height();
    int get_width();

    std::vector<std::vector<char>> getMap();

    char get_position(int x, int y);

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

};

#endif
