#pragma once
#include <vector>
#include "move_engine.hpp"
using namespace std;

class GameEngine
{
    public: 
        GameEngine();
        void print();
        vector<int> getMoveList() {return __getMoveList(false);};
        int playMove(int m);
        int getPlayer();
        vector<int> getBoardChange(int player);
        int getScore();
        int getWhiteScore();
        void reset();
        bool gameEnded();
        void undoLastMove();
        vector<int> getBlackList();
        vector<int> getWhiteList();
        uint64_t getBlackBoard(){return board1[count];}
        uint64_t getWhiteBoard(){return board2[count];}

    private:
        void __updateMoveBoard();
        static MoveEngine move_engine;
        bool game_ended;
        int count;
        vector<int> __getMoveList(bool t = false);
        uint64_t current_moves;
        int player;
        uint64_t board1[62];
        uint64_t board2[62];
        void __flip(int move);
    };