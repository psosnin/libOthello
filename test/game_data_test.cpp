
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

void verifyString(string line) {
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

//test 100,000 game benchmark
TEST(DISABLED_gameDataTest, benchmarkGames) {
    verifyFile("data/data.txt");
}

TEST(DISABLED_gameDataTest, sampleGames01) {
    verifyFile("data/data01.txt");
}

TEST(DISABLED_gameDataTest, sampleGames02) {
    verifyFile("data/data02.txt");
}

TEST(DISABLED_gameDataTest, sampleGames03) {
    verifyFile("data/data03.txt");
}

TEST(DISABLED_gameDataTest, sampleGames04) {
    verifyFile("data/data04.txt");
}

TEST(DISABLED_gameDataTest, sampleGames05) {
    verifyFile("data/data05.txt");
}

TEST(DISABLED_gameDataTest, sampleGames06) {
    verifyFile("data/data06.txt");
}

TEST(DISABLED_gameDataTest, sampleGames07) {
    verifyFile("data/data07.txt");
}

TEST(DISABLED_gameDataTest, sampleGames08) {
    verifyFile("data/data08.txt");
}

TEST(DISABLED_gameDataTest, sampleGames09) {
    verifyFile("data/data09.txt");
}

TEST(DISABLED_gameDataTest, sampleGames10) {
    verifyFile("data/data10.txt");
}

TEST(DISABLED_gameDataTest, sampleGames11) {
    verifyFile("data/data11.txt");
}

TEST(DISABLED_gameDataTest, sampleGames12) {
    verifyFile("data/data12.txt");
}


TEST(DISABLED_gameDataTest, sampleGames13) {
    verifyFile("data/data13.txt");
}

TEST(DISABLED_gameDataTest, sampleGames14) {
    verifyFile("data/data14.txt");
}

TEST(DISABLED_gameDataTest, sampleGames15) {
    verifyFile("data/data15.txt");
}

TEST(gameDataTest, manualTest) {
    verifyString("11 37 43 34 29 20 42 19 45 44 51 22 26 38 18 53 11 52 13 58 21 50 23 12 5 3 4 6 30 2 47 31 39 33 15 46 10 1 40 17 25 16 61 32 24 14 59 60 57 48 7 49 0 8 9 41 56 54 55 63 62");
    verifyString("16 37 43 26 19 18 29 45 33 25 21 44 52 53 34 20 16 50 51 41 46 38 10 40 42 22 39 59 60 47 30 61 58 57 32 49 11 13 12 31 56 48 5 14 54 24 62 17 7 6 9 2 4 8 0 1 3 55 15 23 63");
    verifyString("61 37 45 44 29 38 52 20 46 43 26 53 39 47 55 51 59 42 60 34 31 21 41 58 57 50 25 33 40 16 24 62 18 19 30 22 61 56 49 23 15 48 10 11 13 12 3 5 2 32 4 1 9 6 14 7 63 8 0 17 54");
    verifyString("8 37 43 26 19 34 29 20 21 51 18 11 46 17 25 33 3 12 10 2 4 30 38 45 31 39 47 23 15 22 13 5 52 61 44 53 24 32 50 42 16 58 49 8 40 56 0 41 48 14 57 9 7 55 6 63 1 54 60 59 62");
    verifyString("52 37 45 44 29 38 42 46 55 30 47 31 52 21 20 43 13 59 51 39 23 22 61 58 53 50 34 60 57 11 19 26 15 54 18 12 3 2 1 17 16 4 5 10 25 9 62 63 8 56 49 48 40 14 33 7 6 0 41 32 24");
    verifyString("36 37 43 18 19 26 29 45 33 20 21 30 10 42 51 17 24 2 11 44 13 22 12 5 38 34 25 46 31 39 23 15 53 16 55 4 8 41 40 32 9 0 1 3 14 48 49 56 50 57 59 58 52 60 61 62 47 63 54 7 6");
    verifyString("42 37 43 18 19 26 21 34 44 29 20 45 42 53 17 12 25 51 52 60 62 46 58 13 47 38 39 50 10 33 5 24 57 31 30 55 4 16 11 23 41 22 40 32 14 15 8 3 2 9 59 61 0 1 54 63 56 7 6 48 49");
    verifyString("14 37 43 18 19 26 29 45 21 30 38 20 23 22 13 47 31 42 12 11 34 51 2 4 25 17 33 44 5 6 10 46 50 40 39 15 32 58 41 24 16 8 52 61 60 48 59 53 3 1 49 9 14 57 56 0 7 55 62 54 63");
    verifyString("46 37 43 18 19 26 29 34 25 20 44 41 42 21 40 52 53 51 45 17 58 59 61 60 50 24 10 11 3 33 32 16 12 48 46 13 38 30 39 5 4 22 23 14 8 0 7 9 49 47 1 2 54 63 62 55 6 15 31 56 57");
    verifyString("20 37 45 44 29 38 52 19 20 53 26 17 43 42 18 60 25 33 51 50 24 34 16 22 40 32 41 21 58 12 59 11 61 10 2 49 62 54 63 55 3 46 4 13 23 31 5 15 47 30 39 48 7 6 14 1 0 8 57 9 56");
}