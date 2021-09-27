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
        uint64_t getBlackBoard(){return player1;}
        uint64_t getWhiteBoard(){return player2;}

    private:
        void __updateMoveBoard();
        static MoveEngine move_engine;
        bool game_ended;
        int move_count;
        vector<int> __getMoveList(bool t = false);
        uint64_t current_moves;
        uint64_t player1;
        uint64_t player2; 
        uint64_t past1;
        uint64_t past2;
        uint64_t history1[60];
        uint64_t history2[60];
        void __flip(int move);
    };