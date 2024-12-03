#include <gtest/gtest.h>
#include "../server/game/round_manager.h"


TEST(RoundManagerTest, TestInitializeManagerInitializesParametersCorrectly) {
    RoundManager rm;
    rm.initialize_manager(5);
    
    ASSERT_FALSE(rm.check_end_of_match());
    ASSERT_FALSE(rm.check_end_of_five_rounds());
    ASSERT_EQ(rm.get_duck_winner(), '0');
    ASSERT_EQ(rm.check_match_status(), MATCH_NEXT_ROUND);
}

TEST(RoundManagerTest, TestInitialGetRoundReturnsCorrectRound) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    EXPECT_EQ(rm.get_round(), 1);
}

TEST(RoundManagerTest, TestDeclareRoundWinnerDeclaresCorrectly) {
    RoundManager rm;
    rm.initialize_manager(3);
    rm.declare_round_winner('1');
    
    EXPECT_EQ(rm.get_duck_round_winner(), '1');
    EXPECT_EQ(rm.get_round(), 1);
}

TEST(RoundManagerTest, TestCheckMatchStatusUpdatesCorrectly) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    EXPECT_EQ(rm.check_match_status(), MATCH_NEXT_ROUND);
    EXPECT_EQ(rm.get_round(), 2);
    
    for (int i = 0; i < 2; ++i) {
        rm.check_match_status();
    }
    
    EXPECT_EQ(rm.check_match_status(), 2);
    EXPECT_EQ(rm.get_round(), 5);
}

TEST(RoundManagerTest, TestCheckEndOfFiveRoundsReturnsCorrectly) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    for (int i = 1; i <= 4; ++i) {
        EXPECT_FALSE(rm.check_end_of_five_rounds());
        rm.check_match_status();
    }
    
    EXPECT_TRUE(rm.check_end_of_five_rounds());
}

TEST(RoundManagerTest, TestCheckEndOfMatchReturnsCorrectly) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    rm.declare_round_winner('1');
    rm.declare_round_winner('1');
    rm.declare_round_winner('1');
    
    EXPECT_FALSE(rm.check_end_of_match());
    
    for (int i = 0; i < 5; ++i) {
        rm.check_match_status();
        rm.declare_round_winner('1');
    }
    
    EXPECT_FALSE(rm.check_end_of_match());
}

TEST(RoundManagerTest, TestGetDuckWinnerReturnsCorrectWinner) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    for (int i = 0; i < 10; ++i) {
        rm.declare_round_winner('1');
        rm.check_match_status();
    }
    
    EXPECT_EQ(rm.get_duck_winner(), '1');
}

TEST(RoundManagerTest, TestGetFiveRoundMessageGeneratesCorrectMessage) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    for (int i = 0; i < 5; ++i) {
        rm.check_match_status();
        rm.declare_round_winner('1');
    }
    
    Message msg;
    EXPECT_TRUE(rm.get_five_round_message(msg));
    EXPECT_EQ(msg.type, END_FIVE_ROUNDS);
    EXPECT_EQ(msg.round, 6);
}

TEST(RoundManagerTest, TestGetEndRoundMessageGeneratesCorrectMessage) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    rm.check_match_status();
    rm.declare_round_winner('1');
    
    Message msg;
    EXPECT_TRUE(rm.get_end_round_message(msg));
    EXPECT_EQ(msg.type, END_ROUND);
    EXPECT_EQ(msg.round, 2);
}

TEST(RoundManagerTest, TestGetEndMatchMessageGeneratesCorrectMessage) {
    RoundManager rm;
    rm.initialize_manager(3);
    
    for (int i = 0; i < 10; ++i) {
        rm.declare_round_winner('1');
        rm.check_match_status();
    }
    
    Message msg;
    EXPECT_TRUE(rm.get_end_match_message(msg));
    EXPECT_EQ(msg.type, END_GAME);
    EXPECT_EQ(msg.round, 10);
}
