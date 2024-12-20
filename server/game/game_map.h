
#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include "common/position.h"
#include "common/constants.h"
#include "../level_manager.h"


class GameMap {

private:
    int width;
    int height;
    std::vector<std::vector<char>> map;


public:
    GameMap(int width, int height);

    bool placeDuck(Position pos, char duck_id);

    bool is_duck_touching_floor(int x, int y);

    bool bullet_hit_other_duck(char e, char duck_shooting);
    bool is_element_touching_floor(int x, int y, int size_x, int size_y);

    char duck_in_position(int x, int y, int size_x, int size_y);

    void clear_map();

    void setEscenario();

    void set_escenario_for_round(MapConfig mapConfig);

    bool canMoveDuckTo(int x, int y, char duck_id);

    void setBulletNewPosition(Position pos);

    void cleanBulletOldPosition(Position pos);

    Position try_move_bullet_to(Position old_position, Position new_position, char duck_id, bool& hit_something);

    Position try_move_bouncing_laser_to(Position old_position, Position new_position, char duck_id, bool& hit_something, bool& hit_platform, bool& hit_x);

    Position move_duck_to(Position old_position, Position new_position, char duck_id);

    void cleanDuckOldPosition(int x, int y);

    void setDuckNewPosition(int x, int y, char duck_id);

    char at(Position position);

    bool can_move_projectile(Position position, int size_x, int size_y);

    void move_projectile(Position position, Position speed, int size_x, int size_y);

    void clean_projectile_old_position(Position position, int size_x, int size_y);

    void set_projectile_new_position(Position position,  int size_x, int size_y, char letter);

    Position try_move_projectile_to(Position old_position, Position new_position, int size_x, int size_y, char duck_id, bool& hit_something);

    int get_height();
    int get_width();

    std::vector<std::vector<char>> getMap();

    bool duckIsOverVoid(int x, int y);

    bool duckIsOverBullet(Position position);

    bool duckIsOverBanana(Position position, Position &banana_position);

    //METODO TEMPORAL: Imprime el mapa en la consola
    void printMap() const;

    void tellMap() const;

    Position try_move_banana(Position old_position, Position speed, bool& hit_void);

    Position try_move_grenade(Position old_position, Position speed, bool& hit_void);

    bool is_throwable_touching_floor(Position position, int size_x, int size_y);
    void placeBox(Position pos);
    void removeBox(Position pos);
    bool isCollisionWithBox(Position pos, int size_x, int size_y);
    bool boxIsOverBullet(Position position);


};

#endif
