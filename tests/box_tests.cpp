#include "gtest/gtest.h"
#include "../server/game/box.h"
#include <memory>

TEST(BoxTest, BoxIsInitializedCorrectly) {
    Position pos = {10, 20};
    GameMap map = GameMap(30, 30);
    uint16_t item_id = 1;
    Box box(pos, 1, &map, item_id);

    EXPECT_EQ(box.getPosition().x, 10);
    EXPECT_EQ(box.getPosition().y, 20);
    EXPECT_EQ(box.getItemId(), item_id);
    EXPECT_FALSE(box.isDestroyed());
}

TEST(BoxTest, DestroyBox) {
    Position pos = {10, 20};
    GameMap map = GameMap(30, 30);
    uint16_t item_id = 1;
    Box box(pos, 1, &map, item_id);
    std::vector<std::shared_ptr<Item>> items;

    EXPECT_FALSE(box.isDestroyed());
    box.destroy(items);
    EXPECT_TRUE(box.isDestroyed());

    EXPECT_EQ(items.size(), 1);
}

TEST(BoxTest, DestroyBoxTwiceDoesNotAddItemTwice) {
    Position pos = {10, 20};
    GameMap map = GameMap(30, 30);
    uint16_t item_id = 1;
    Box box(pos, 1, &map, item_id);
    std::vector<std::shared_ptr<Item>> items;

    box.destroy(items);
    EXPECT_TRUE(box.isDestroyed());

    box.destroy(items);
    EXPECT_TRUE(box.isDestroyed());
    EXPECT_EQ(items.size(), 1);
}

TEST(BoxTest, GetBoxMessageContainsRightFields) {
    Position pos = {10, 20};
    GameMap map = GameMap(30, 30);
    uint16_t item_id = 1;
    Box box(pos, 1, &map, item_id);
    Message msg;

    box.getBoxMessage(msg);
    EXPECT_EQ(msg.type, BOX_DESTROYED);
    EXPECT_EQ(msg.box_id, 1);
    EXPECT_EQ(msg.item_id, item_id);
}

TEST(BoxTest, UpdateLifeBoxOverBullet) {
    Position pos = {10, 20};
    GameMap map = GameMap(30, 30);
    uint16_t item_id = 1;
    Box box(pos, 1, &map, item_id);
    std::vector<std::shared_ptr<Item>> items;

    map.placeBox(pos);
    map.setBulletNewPosition(pos);
    EXPECT_TRUE(map.boxIsOverBullet(pos));

    EXPECT_TRUE(box.update_life(items));
    EXPECT_TRUE(box.isDestroyed());
    EXPECT_EQ(items.size(), 1);
}

TEST(BoxTest, UpdateLifeBoxNotOverBullet) {
    Position pos = {10, 20};
    GameMap map = GameMap(30, 30);
    uint16_t item_id = 1;
    Box box(pos, 1, &map, item_id);
    std::vector<std::shared_ptr<Item>> items;

    EXPECT_FALSE(map.boxIsOverBullet(pos));

    EXPECT_FALSE(box.update_life(items));
    EXPECT_FALSE(box.isDestroyed());
    EXPECT_EQ(items.size(), 0);
}
