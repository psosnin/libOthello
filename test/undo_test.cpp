#include "game_engine.hpp"
#include <string>
#include <vector>
#include "gtest/gtest.h"
using namespace std;
/*Passing turn from this position, then undoing the move returns error

black1 = 5706078390811935486
white1 = 4238681869073665
black2 = 5706087186904957694
white2 = 1978085962366209*/

TEST(undoTest, one) {
    uint64_t black, white;
    GameEngine game = GameEngine();
    vector<int> moves = {26, 20,13,42,29,19,12,21,30,6,5,4,3,2,11,10,9,18,17,0,1,24,7,8,16,25,32,40,33,34,
                         41,43,44,48,49,22,14,53,52,15,23,57,56,58,59,50,62,51,-1,61};
    for (int i : moves) {
        black = game.getBlackBoard();
        //white = game.getWhiteBoard();
        int m = game.playMove(i);
        cout << black << endl;
        EXPECT_TRUE(m != -3);
        game.undoLastMove();
        //EXPECT_EQ(white, game.getWhiteBoard());
        EXPECT_EQ(black, game.getBlackBoard());
        game.playMove(i);
        EXPECT_TRUE(m != -3);
    }
}