#include <gtest/gtest.h>
#include "../server/game/game.h"



TEST(GameTest, TestGameIsInitializedCorrectly) {
    bool is_over = false;
    GameQueueMonitor monitor;
    Game game(1, monitor, is_over);

    auto val = std::shared_ptr<Executable>();
    EXPECT_EQ(game.get_game_queue().try_pop(val), false);
    EXPECT_TRUE(game.ducks.empty());
    EXPECT_TRUE(game.items_on_floor.empty());
}

TEST(GameTest, TestAddThrowedWeapon) {
    bool is_over = false;
    GameQueueMonitor monitor;
    Game game(1, monitor, is_over);

    Grenade grenade = Grenade();
    game.add_throwed_weapon((Weapon*)&grenade);
    
    EXPECT_EQ(game.throwed_weapons.size(), 1);
}

TEST(GameTest, TestGetDuckByPositionReturnsTheCorrectDuck) {
    bool is_over = false;
    GameQueueMonitor monitor;
    Game game(1, monitor, is_over);

    Position pos(1, 2);
    Duck* null_duck = game.getDuckByPosition(pos);
    EXPECT_EQ(null_duck, nullptr);

    Duck duck = Duck('1', 1, 2, &game.map);
    game.ducks.emplace_back(duck);
    game.map.setDuckNewPosition(1, 2, '1');

    Duck* duck_from_position = game.getDuckByPosition(pos);
    EXPECT_EQ(duck.get_id(), duck_from_position->get_id());
}

TEST(GameTest, TestGetDuckByIDReturnsTheCorrectDuck) {
    bool is_over = false;
    GameQueueMonitor monitor;
    Game game(1, monitor, is_over);

    Duck duck = Duck('1', 1, 2, &game.map);
    game.ducks.emplace_back(duck);

    Duck* duck_from_id = game.getDuckById('1');
    EXPECT_EQ(duck.get_id(), duck_from_id->get_id());
}


