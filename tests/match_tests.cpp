#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../server/match.h"


#define MAX_PLAYERS 6
#define MIN_PLAYERS 2

// Set Up provisorio
std::shared_ptr<Player> initializer(int port_number) {
    ServerProtocol protocol = ServerProtocol(Socket(std::to_string(port_number).c_str()));
    auto queue = Queue<std::shared_ptr<Executable>>(10);
    auto player = std::make_shared<Player>(std::move(protocol), 0, queue);
    return player;
}

TEST(MatchTest, MatchStartsEmpty) {
    Match match(1);
    EXPECT_EQ(match.get_players_count(), 0);
}

TEST(MatchTest, AddsAndRemovePlayerSuccessfully) {
    Match match(1);
    auto player = initializer(1234);
    match.add_player(player);
    match.remove_player();
    EXPECT_EQ(match.get_players_count(), 0);
}

TEST(MatchTest, RemovePlayerFromEmptyMatch) {
    Match match(1);
    match.remove_player();
    EXPECT_EQ(match.get_players_count(), 0);
}

TEST(MatchTest, CannotStartWithInsufficientPlayers) {
    Match match(1);
    EXPECT_FALSE(match.is_able_to_start());
}

TEST(MatchTest, MatchAvailableBeforeStart) {
    Match match(1);
    EXPECT_TRUE(match.is_match_available());
}
