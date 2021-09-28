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
        void redoNextMove();
        vector<int> getBlackList();
        vector<int> getWhiteList();
        uint64_t getBlackBoard(){return board1[count];}
        uint64_t getWhiteBoard(){return board2[count];}

    private:
        void __updateMoveBoard();
        static MoveEngine move_engine;
        bool game_ended;
        int count; // index of the current move
        int undone; // no of moves undone since last move was played
        vector<int> __getMoveList(bool t = false);
        uint64_t current_moves; // holds a bitboard of the positions of all current valid moves
        int player; // holds player to move, 1 for balck, -1 for white
        uint64_t board1[62];
        uint64_t board2[62];
        void __flip(int move);
    };