
//Test some example games
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <game_engine.hpp>
#include "gtest/gtest.h"

using namespace std;

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
    game.reset();
}

void verifyFile(string dfile) {
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
}

//test 100,000 game benchmark
TEST(gameDataTest, benchmarkGames) {
    verifyFile("tests/data/data.txt");
}

TEST(gameDataTest, sampleGames01) {
    verifyFile("tests/data/data01.txt");
}

TEST(gameDataTest, sampleGames02) {
    verifyFile("tests/data/data02.txt");
}

TEST(gameDataTest, sampleGames03) {
    verifyFile("tests/data/data03.txt");
}

TEST(gameDataTest, sampleGames04) {
    verifyFile("tests/data/data04.txt");
}

TEST(gameDataTest, sampleGames05) {
    verifyFile("tests/data/data05.txt");
}

TEST(gameDataTest, sampleGames06) {
    verifyFile("tests/data/data06.txt");
}

TEST(gameDataTest, sampleGames07) {
    verifyFile("tests/data/data07.txt");
}

TEST(gameDataTest, sampleGames08) {
    verifyFile("tests/data/data08.txt");
}

TEST(gameDataTest, sampleGames09) {
    verifyFile("tests/data/data09.txt");
}

TEST(gameDataTest, sampleGames10) {
    verifyFile("tests/data/data10.txt");
}

TEST(gameDataTest, sampleGames11) {
    verifyFile("tests/data/data11.txt");
}

TEST(gameDataTest, sampleGames12) {
    verifyFile("tests/data/data12.txt");
}


TEST(gameDataTest, sampleGames13) {
    verifyFile("tests/data/data13.txt");
}

TEST(gameDataTest, sampleGames14) {
    verifyFile("tests/data/data14.txt");
}

TEST(gameDataTest, sampleGames15) {
    verifyFile("tests/data/data15.txt");
}