#pragma once
#include <set>
#include "move_engine.hpp"
using namespace std;

class GameEngine
{
    public: 
        GameEngine();
        void print();
        set<int> getMoveList(bool t = false);
        int playMove(int m);
        int returnWinner();
        int getPlayer();
        set<int> getBoardChange(int player);
        int getScore();
        void reset();
        bool gameEnded();

    private:
        void updateMoveBoard();
        static MoveEngine move_engine;
        bool game_ended;
        int move_count;
        uint64_t current_moves;
        uint64_t player1;
        uint64_t player2; 
        uint64_t past1;
        uint64_t past2;
        void flip(int move);
    };