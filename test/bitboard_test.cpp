// Test for bitboard operations
#include <bitboard_ops.hpp>
#include "gtest/gtest.h"

#define top_left     0x8000000000000000
#define top_right    0x0100000000000000
#define bottom_left  0x0000000000000080
#define bottom_right 0x0000000000000001

TEST(bbTest, testFlips) {
    uint64_t board = 3;
    // Check all board with a single piece on, to check
    // each bit is mapped correctly
    for (int i = 0; i < 64; i++) {
        board = board << 1;

        // Check reversibility
        EXPECT_EQ(board, flipV(flipV(board)));
        EXPECT_EQ(board, flipH(flipH(board)));
        EXPECT_EQ(board, flipD(flipD(board)));
        EXPECT_EQ(board, flipAD(flipAD(board)));

        // Check commutivity
        EXPECT_EQ(flipV(flipH(board)), flipH(flipV(board)));
        EXPECT_EQ(flipD(flipAD(board)), flipAD(flipD(board)));
        
        // Check combinations
        EXPECT_EQ(flipH(flipD(board)), flipV(flipAD(board)));
        EXPECT_EQ(flipD(flipH(board)), flipAD(flipV(board)));
    }

    // Check specific cases:
    EXPECT_EQ(top_right, flipV(bottom_right));
    EXPECT_EQ(top_right, flipH(top_left));
    EXPECT_EQ(top_left, flipD(top_left));
    EXPECT_EQ(top_left, flipAD(bottom_right));
}

TEST(bbTest, testRotations) {
    uint64_t board = 3;
    // Check all board with a single piece on, to check
    // each bit is mapped correctly
    for (int i = 0; i < 64; i++) {
        board = board << 1;

        // Check reversibility
        EXPECT_EQ(board, rotC90(rotAC90(board)));
        EXPECT_EQ(board, rotAC90(rotC90(board)));
        EXPECT_EQ(board, rotC45(rotAC45(board)));
        EXPECT_EQ(board, rotAC45(rotC45(board)));

        // Check full rotation
        EXPECT_EQ(board, rotC90(rotC90(rotC90(rotC90(board)))));
        EXPECT_EQ(board, rotAC90(rotAC90(rotAC90(rotAC90(board)))));

        // Check commutivity
        EXPECT_EQ(rotC90(rotAC90(board)), rotAC90(rotC90(board)));
        EXPECT_EQ(rotC90(rotAC90(board)), rotAC90(rotC90(board)));
    }
    // Check specific cases:
    EXPECT_EQ(top_right, rotC90(top_left));
    EXPECT_EQ(top_right, rotAC90(bottom_right));
}

TEST(bbTest, testPlace) {};
TEST(bbTest, testGets) {};
TEST(bbTest, testSets) {};
