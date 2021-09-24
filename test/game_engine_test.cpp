// Test for bitboard operations
#include <game_engine.hpp>
#include <string>
#include <vector>
#include "gtest/gtest.h"

#define top_left     0x8000000000000000
#define top_right    0x0100000000000000
#define bottom_left  0x0000000000000080
#define bottom_right 0x0000000000000001

void verify(vector<int> moves, int score) {
    static GameEngine game = GameEngine();
    int move_result = 0;
    for (int i : moves) {
        if (i == -9) break;
        move_result = game.playMove(i);
        while (move_result == -1) {
            move_result = game.playMove(i);
        }
        EXPECT_NE(move_result, -3);
    }
    EXPECT_EQ(score, game.getScore());
    game.reset();
}

TEST(gameEngineTest, testGetBoardChange) {
    vector<int> moves = {37, 43, 34, 29, 20, 42, 19, 45, 44, 51, 22, 26, 38, 18, 53, 11, 52, 13, 58, 21, 50, 23, 12, 5 ,3, 4, 6, 30, 2, 47, 31, 39, 33, 15, 46, 10, 1 ,40, 17, 25, 16, 61, 32, 24, 14, 59, 60, 57, 48, 7 ,49 ,0 ,8, 9, 41, 56, 54, 55, 63, 62};
    int score = 11;
    verify(moves, score);
}