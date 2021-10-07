
//Test some example games
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <game_engine.hpp>
#include "gtest/gtest.h"

using namespace std;
static int no_games = 0;

void verifyGame(vector<int> moves, int score) {
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
    no_games++;
    game.reset();
}

void verifyFile(string dfile) {
    no_games = 0;
    ifstream data;
    data.open(dfile);
    string line;
    while(getline(data, line)) {
        int score = 0;
        vector<int> moves = {};
        istringstream is(line);
        is >> score;
        int n;
        while (is >> n) {
            moves.push_back(n);
        }
        verifyGame(moves, score);
    }
    data.close();
    cout << "[   GAMES  ] " << no_games << " games tested" << endl;
}

//test 100,000 game benchmark
TEST(DISABLED_gameDataTest, benchmarkGames) {
    verifyFile("data/data.txt");
}

TEST(DISABLED_gameDataTest, testCorrect01) {
    verifyFile("data/data01.txt");
}

TEST(DISABLED_gameDataTest, testCorrect02) {
    verifyFile("data/data02.txt");
}

TEST(DISABLED_gameDataTest, testCorrect03) {
    verifyFile("data/data03.txt");
}

TEST(DISABLED_gameDataTest, testCorrect04) {
    verifyFile("data/data04.txt");
}

TEST(DISABLED_gameDataTest, testCorrect05) {
    verifyFile("data/data05.txt");
}

TEST(DISABLED_gameDataTest, testCorrect06) {
    verifyFile("data/data06.txt");
}

TEST(DISABLED_gameDataTest, testCorrect07) {
    verifyFile("data/data07.txt");
}

TEST(DISABLED_gameDataTest, testCorrect08) {
    verifyFile("data/data08.txt");
}

TEST(DISABLED_gameDataTest, testCorrect09) {
    verifyFile("data/data09.txt");
}

TEST(DISABLED_gameDataTest, testCorrect10) {
    verifyFile("data/data10.txt");
}

TEST(DISABLED_gameDataTest, testCorrect11) {
    verifyFile("data/data11.txt");
}

TEST(DISABLED_gameDataTest, testCorrect12) {
    verifyFile("data/data12.txt");
}


TEST(DISABLED_gameDataTest, testCorrect13) {
    verifyFile("data/data13.txt");
}

TEST(DISABLED_gameDataTest, testCorrect14) {
    verifyFile("data/data14.txt");
}

TEST(DISABLED_gameDataTest, testCorrect15) {
    verifyFile("data/data15.txt");
}