#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../server/game/duck.h"
#include "../server/game/game_map.h"
#include "../server/game/item.h"
#include "../server/guns/weapon.h"


#include <cstdint>
#include <string>
#include <condition_variable>


#include <typeinfo>

const int HELMET_BROKE = 0;
const int ARMOR_BROKE = 1;
const int DEAD = 2;

// Test de pickUpItem
TEST(DuckTest, TestPickedUpItemIsOnHandAfter) {
    auto item = std::make_shared<Item>(0);
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);
    EXPECT_TRUE(duck.pickUpItem(item));
    EXPECT_EQ(item.get(), duck.getItemOnHand().get());

    duck.useOnHand();
    EXPECT_EQ(nullptr, duck.getItemOnHand());
}

TEST(DuckTest, TestCheckDuckIsInAir) {
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);

    EXPECT_TRUE(duck.is_in_air());
}

TEST(DuckTest, TestUpdateLifeHelmetBroken) {
    Position pos = {10, 20};
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);
    duck.setHelmet(std::make_shared<Helmet>());
    duck.setArmor(std::make_shared<Armor>());


    map.setBulletNewPosition(pos);
    int result = duck.update_life();
    
    ASSERT_EQ(result, HELMET_BROKE);
    ASSERT_EQ(duck.helmet, nullptr);
    ASSERT_NE(duck.armor, nullptr);

    result = duck.update_life();
    
    ASSERT_EQ(result, ARMOR_BROKE);
    ASSERT_EQ(duck.armor, nullptr);
    ASSERT_EQ(duck.helmet, nullptr);

    result = duck.update_life();

    ASSERT_EQ(result, DEAD);    
}

// Test de update_position (cuando el pato se mueve con resbalón)
TEST(DuckTest, TestUpdatePositionSlippy) {
    Position pos = {10, 20};
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);

    duck.speed_x = 2;
    duck.is_slippy = true;

    
    map.duckIsOverBanana(pos, pos);
    Position result = map.move_duck_to(pos, Position {12, 20}, '1');
    ASSERT_EQ(result.x, 12);
}

// Test de retroceso
TEST(DuckTest, TestDisparar) {
    auto mock_weapon = std::make_shared<Magnum>();
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);

    // Pato no dispara al no tener arma
    ASSERT_EQ(duck.disparar(), false);

    duck.setWeapon(mock_weapon);

    // Pato dispara al tener arma
    ASSERT_EQ(duck.disparar(), true); 
    ASSERT_EQ(duck.getPosition().x, 9);
}

// Test de retroceso
TEST(DuckTest, TestNoBullets) {
    auto mock_weapon = std::make_shared<DuelPistol>();
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);

    duck.setWeapon(mock_weapon);

    // Pato dispara al tener arma
    ASSERT_EQ(duck.disparar(), true); 

    // Pato no dispara al no tener municiones
    ASSERT_EQ(duck.disparar(), false); 
}

// Test de get_duck_dead_message (cuando el pato está muerto)
TEST(DuckTest, TestDuckDeadMessage) {
    Position pos = {10, 20};
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);


    map.setBulletNewPosition(pos);
    int result = duck.update_life();

    Message msg;
    bool msg_result = duck.get_duck_dead_message(msg);

    ASSERT_TRUE(msg_result);
    ASSERT_EQ(msg.type, KILL_DUCK);
    ASSERT_EQ(msg.player_id, 1);
}

// Test de get_duck_dead_message (cuando el pato está muerto)
TEST(DuckTest, TestDuckBrokenHelmetMessage) {
    Position pos = {10, 20};
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);
    duck.setHelmet(std::make_shared<Helmet>());


    map.setBulletNewPosition(pos);
    int result = duck.update_life();

    Message msg;
    bool msg_result = duck.get_duck_broke_helmet_message(msg);

    ASSERT_TRUE(msg_result);
    ASSERT_EQ(msg.type, HELMET_BROKEN);
    ASSERT_EQ(msg.player_id, 1);
}

// Test de get_duck_dead_message (cuando el pato está muerto)
TEST(DuckTest, TestDuckBrokenArmorMessage) {
    Position pos = {10, 20};
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);
    duck.setArmor(std::make_shared<Armor>());


    map.setBulletNewPosition(pos);
    int result = duck.update_life();

    Message msg;
    bool msg_result = duck.get_duck_broke_armor_message(msg);

    ASSERT_TRUE(msg_result);
    ASSERT_EQ(msg.type, ARMOR_BROKEN);
    ASSERT_EQ(msg.player_id, 1);
}

// Test de get_duck_position_message (cuando el pato ha cambiado de posición)
TEST(DuckTest, TestDuckUpdatedPositionMessage) {
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);

    duck.update_position();

    Message msg;

    ASSERT_EQ(duck.get_duck_position_message(msg), true);

    ASSERT_EQ(msg.type, DUCK_POS_UPDATE);
    ASSERT_EQ(msg.player_id, 1);
    ASSERT_EQ(msg.duck_x, duck.getPosition().x);
    ASSERT_EQ(msg.duck_y, duck.getPosition().y);
    // podría probar todos los otros campos pero serían el default
}

TEST(DuckTest, TestDuckPositionNotUpdatedMessage) {
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);

    Message msg;

    ASSERT_EQ(duck.get_duck_position_message(msg), true);
    ASSERT_EQ(duck.get_duck_position_message(msg), false);
}

TEST(DuckTest, TestWeaponIDIsExpected) {
    auto weapon = std::make_shared<Banana>();
    GameMap map(60, 60);
    Duck duck('1', 10, 20, &map);
    duck.setWeapon(weapon);

    ASSERT_EQ(duck.getWeaponId(), BANANA_ID);
}