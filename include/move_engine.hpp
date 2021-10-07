#pragma once
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class MoveEngine { 
    public:
        MoveEngine();
        //a function that returns a bitboard of opponents pieces that are flipped on a move. 
        //player and opponent are the current bitboards, move is an integer value representing 
        //the number of the bit that the move is on
        uint64_t getFlipBoard(uint64_t player, uint64_t opponent, int move);
        //returns the number of bits set in the given bitboard
        int getNoBits(uint64_t board);
        //a function that decomposes a bit board into a set of integers representing the position of 
        //each bit on the bitboard
        vector<int> getBitList(uint64_t move_board);
        //gets all the possible moves on the board and returns it as a new bitboard
        uint64_t getMoveBoard(uint64_t player, uint64_t opponent);
        //modifies player and opponent's bitboards as the move int was played
        void playMove(uint64_t * player, uint64_t * opponent, int move);

    private:
        //a function that returns a key into the flip_map given a black row, a white row and a move
        uint64_t __flipMapKey(int player, int opponent, int move);
        //precomputes the possible valid moves and their corresponding flips for any row state 
        //given by player and opponent
        void __computeMoves(unsigned int player, unsigned int opponent);
        //precomputes the move table for all moves for every possible configuration of white and black counters 
        //on a row. the table is indexed by a 16 bit number -> the first 8 bits are the player's row state, the 
        //last 8 are the opponent's row state
        void __generateMoveTable();
        //generates an index into the move table
        int __moveArrayIndex(int player, int opponent);
        //gets all possible moves on the board in the hoirizontal direction
        uint64_t __getHMoves(uint64_t player, uint64_t opponent);
        //gets all possible moves on the board in the vertical direction
        uint64_t __getVMoves(uint64_t player, uint64_t opponent);
        //gets all possible moves on the board in the direction of the leading diagonal
        uint64_t __getDMoves(uint64_t player, uint64_t opponent);
        //gets all the possible moves on the board in the direction of the anti-diagonal
        uint64_t __getADMoves(uint64_t player, uint64_t opponent);
        //a function that takes the bit of a bitboard and returns the x,y index MSB (0,0) 
        void __getXYIndex(int bit, int * xy);
        //a function that takes an (x,y) index and returns the (diagonal, antidiagonal) indices of that coordinate.
        void __getDiagonalIndex(int * xy, int * diag);
        struct MoveTable {
            char moves[256*256];
            unordered_map<int, char> flip_map;
        } move_table;
};