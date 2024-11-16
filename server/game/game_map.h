
#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include "common/position.h"
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

    bool bullet_hit_other_duck(char e, char duck_shooting);
    bool is_element_touching_floor(int x, int y, int size_x, int size_y);

    char duck_in_position(int x, int y, int size_x, int size_y);

    void setEscenario();

    bool canMoveDuckTo(int x, int y, char duck_id);

    void setBulletNewPosition(Position pos);

    void cleanBulletOldPosition(Position pos);

    Position try_move_bullet_to(Position old_position, Position new_position, char duck_id, bool& hit_something);

    Position try_move_bouncing_laser_to(Position old_position, Position new_position, char duck_id, bool& hit_something, bool& hit_platform, bool& hit_x);

    Position move_duck_to(Position old_position, Position new_position, char duck_id);

    void cleanDuckOldPosition(int x, int y);

    void setDuckNewPosition(int x, int y, char duck_id);

    void setBulletNewPosition(int x, int y);
    void cleanBulletOldPosition(int x, int y);

    char at(Position position);

    bool can_move_projectile(int x, int y, int size_x, int size_y);

    int projectile_hits_duck(int x, int y);

    void move_projectile(int position_x, int position_y, int speed_x, int speed_y, int size_x, int size_y);

    void clean_projectile_old_position(int x, int y, int size_x, int size_y);

    void set_projectile_new_position(int x, int y,  int size_x, int size_y);

    int get_height();
    int get_width();

    std::vector<std::vector<char>> getMap();

    bool duckIsOverVoid(int x, int y);

    bool duckIsOverBullet(Position position);

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

    void tellMap() const;

};

#endif
