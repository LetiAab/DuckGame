#include <gtest/gtest.h>
#include "../server/game/game_map.h"

const char EMPTY = ' ';
const char DUCK_1 = '1';
const char DUCK_2 = '2';
const char DUCK_3 = '3';
const char DUCK_4 = '4';
const char DUCK_5 = '5';
const char DUCK_6 = '6';
const char PLATFORM = 'P';
const char ITEM = 'I';

const char VOID = 'V';

const char BOX = 'B';
const char BANANA = 'b';

const char BULLET = '*';


TEST(GameMapTest, InitialMapSize) {
    GameMap game_map(50, 50);
    ASSERT_EQ(game_map.get_width(), 50);
    ASSERT_EQ(game_map.get_height(), 50);
}

TEST(GameMapTest, BulletMovement) {
    GameMap game_map(50, 50);
    Position pos = {3, 3};
    game_map.setBulletNewPosition(pos);
    
    for (int y = pos.y; y < pos.y + BULLET_SIZE_Y; ++y) {
        for (int x = pos.x; x < pos.x + BULLET_SIZE_X; ++x) {
            ASSERT_EQ(game_map.getMap()[y][x], BULLET);
        }
    }

    game_map.cleanBulletOldPosition(pos);
    
    for (int y = pos.y; y < pos.y + BULLET_SIZE_Y; ++y) {
        for (int x = pos.x; x < pos.x + BULLET_SIZE_X; ++x) {
            ASSERT_EQ(game_map.getMap()[y][x], EMPTY);
        }
    }
}

TEST(GameMapTest, BulletHitDuck) {
    GameMap game_map(50, 50);
    ASSERT_TRUE(game_map.bullet_hit_other_duck(DUCK_1, '2'));
    ASSERT_FALSE(game_map.bullet_hit_other_duck(DUCK_1, '1'));
    ASSERT_FALSE(game_map.bullet_hit_other_duck(PLATFORM, '2'));
}

TEST(GameMapTest, TryMoveBulletTo) {
    GameMap game_map(50, 50);
    Position old_pos = {5, 5};
    Position new_pos = {7, 5};  // Intento mover la bala
    bool hit_something = false;
    Position result = game_map.try_move_bullet_to(old_pos, new_pos, '1', hit_something);
    
    ASSERT_EQ(result.x, 7);
    ASSERT_EQ(result.y, 5);
}

TEST(GameMapTest, IsThrowableTouchingFloor) {
    GameMap game_map(50, 50);
    Position pos = {3, 3};
    ASSERT_FALSE(game_map.is_throwable_touching_floor(pos, 1, 1));

    game_map.set_platform(Position {3, 4});
    ASSERT_TRUE(game_map.is_throwable_touching_floor(pos, 1, 1));
}

TEST(GameMapTest, MoveDuckTo) {
    GameMap game_map(50, 50);
    Position old_pos = {1, 1};
    Position new_pos = {4, 4};
    char duck_id = '1';

    Position result = game_map.move_duck_to(old_pos, new_pos, duck_id);
    
    ASSERT_EQ(result.x, 4);
    ASSERT_EQ(result.y, 4);
}

TEST(GameMapTest, DuckIsOverVoid) {
    GameMap game_map(50, 50);
    Position pos = {3, 3};
    ASSERT_FALSE(game_map.duckIsOverVoid(pos.x, pos.y));
    
    game_map.set_void(Position {3, 15});
    ASSERT_TRUE(game_map.duckIsOverVoid(pos.x, pos.y));
}
