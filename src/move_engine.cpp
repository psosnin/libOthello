#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
#include <bit>
#include "bitboard_ops.hpp"
#include "move_engine.hpp"

using namespace std;

MoveEngine::MoveEngine() {
    __generateMoveTable();
}

//a function that returns a key into the flip_map given a black row, a white row and a move
uint64_t MoveEngine::__flipMapKey(int player, int opponent, int move) {
        return (player&0xff) << 16 | (opponent&0xff) << 8 | (move&0xff);
}

//precomputes the possible valid moves and their corresponding flips for any row state given by player and opponent
void MoveEngine::__computeMoves(unsigned int player, unsigned int opponent) {
    //black to move
    bitset<8> pl = player;
    bitset<8> op = opponent;
    bitset<8> move;
    bitset<8> flipped = 0;

    for (int i = 0; i < 8; i++) {
        if (pl[i] == 1 and pl[i+1] == 0 and op[i+1] == 1) {
            flipped.reset();
            for (int j = i + 1; j < 8; j++) {
                flipped[j] = 1;
                if (op[j] == 0) {
                    flipped[j] = 0;
                    move[j] = 1;
                    move_table.flip_map[__flipMapKey(pl.to_ulong(), op.to_ulong(), (0b1 << j))] |= flipped.to_ulong();
                    break;
                }
            }
            
        }
    }
    for (int i = 7; i > 0; i--) {
        if (pl[i] == 1 and pl[i-1] == 0 and op[i-1] == 1) {
            flipped.reset();
            for (int j = i - 1; j >= 0; j--) {
                flipped[j] = 1;
                if (op[j] == 0) {
                    flipped[j] = 0;
                    move[j] = 1;
                    move_table.flip_map[__flipMapKey(pl.to_ulong(), op.to_ulong(), (0b1 << j))] |= flipped.to_ulong();
                    break;
                }
            }
            
        }
    }
    move &= ~(player | opponent);
    move_table.moves[(player<<8) | opponent] = (char)move.to_ulong();
}

//precomputes the move table for all moves for every possible configuration of white and black counters on a row.
//the table is indexed by a 16 bit number -> the first 8 bits are the player's row state, the last 8 are the opponent's row state
void MoveEngine::__generateMoveTable() {
    //generate each possible combination of black and white counters in a row of 8, then compute the moves for this combination
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if ((i & j) == 0) {
               __computeMoves(i,j);
            }
        }
    }
}

//generates an index into the move table
int MoveEngine::__moveArrayIndex(int player, int opponent){return (player << 8) | opponent ;}

//gets all possible moves on the board in the hoirizontal direction
uint64_t MoveEngine::__getHMoves(uint64_t player, uint64_t opponent){
    //generate move for player given the board states stored in player and opponent
    uint64_t moves = 0;
    for (int i = 0; i < 8; i++) {
        moves |= ((uint64_t)move_table.moves[__moveArrayIndex(((player >> 8*i) & 0xff), ((opponent >> 8*i) & 0xff))] & 0xff) << i*8;
    }
    return moves & (~(player | opponent));
}

//gets all possible moves on the board in the vertical direction
uint64_t MoveEngine::__getVMoves(uint64_t player, uint64_t opponent){
    return rotAC90(__getHMoves(rotC90(player), rotC90(opponent)));
}

//gets all possible moves on the board in the direction of the leading diagonal
uint64_t MoveEngine::__getDMoves(uint64_t player, uint64_t opponent) {
    uint64_t rplayer = rotAC45(player);
    uint64_t ropponent = rotAC45(opponent); 
    unsigned int shift[11] = {32,  24,   16,   8,    0,    56,   48,   40,   32,   24,   16};
    unsigned int mask[11] = {0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};
    uint64_t moves = 0;
    for (int i = 0; i < 11; i++ ) {
            unsigned int m = mask[i];
            unsigned int s = shift[i];
            moves |= (((uint64_t)move_table.moves[__moveArrayIndex((rplayer >> s) & m, (ropponent >> s)   & m)] & m) << s);
            }
    return rotC45(moves) & (~(player | opponent));
}

//gets all the possible moves on the board in the direction of the anti-diagonal
uint64_t MoveEngine::__getADMoves(uint64_t player, uint64_t opponent) {
    return flipV(__getDMoves(flipV(player), flipV(opponent)));
}

//gets all the possible moves on the board
uint64_t MoveEngine::getMoveBoard(uint64_t player, uint64_t opponent) {
    return __getVMoves(player, opponent) | __getHMoves(player, opponent) | __getDMoves(player, opponent) | __getADMoves(player, opponent);
}

//a function that decomposes a bit board into a set of integers representing the position of 
//each bit on the bitboard
vector<int> MoveEngine::getBitList(uint64_t move_board) {
    vector<int> move_indices;
     while (move_board != 0){
        uint64_t current_move = __bit_floor(move_board);
        move_board = move_board - current_move;
        move_indices.push_back(log2(current_move));
    } 
    return move_indices;
}

//a function that takes the bit of a bitboard and returns the x,y index MSB (0,0) 
int * MoveEngine::__getXYIndex(int bit) {
    int * xy = new int[2];
    xy[0] = (63-bit)%8;
    xy[1] = (63-bit)/8;
    return xy;
}

//a function that takes an (x,y) index and returns the (diagonal, antidiagonal) indices of that coordinate.
int * MoveEngine::__getDiagonalIndex(int * xy) {
    int * di = new int[2];
    di[0] = 5 + xy[1] - xy[0];
    di[1] =  -2 + xy[1] + xy[0];
    return di;
}


//a function that returns a bitboard of opponents pieces that are flipped on a move. player and opponent are the current bitboards, move is an integer value representing the number of the bit that the move is on
uint64_t MoveEngine::getFlipBoard(uint64_t player, uint64_t opponent, int move){
    // get x, y, d and ad
    int * xy = __getXYIndex(move);
    int * diag = __getDiagonalIndex(xy);
    uint64_t move_board = (uint64_t)0b1 << move;
    //set row
    uint64_t flip_board  = setRow(0, move_table.flip_map[__flipMapKey(getRow(player, xy[1]), getRow(opponent, xy[1]), getRow(move_board, xy[1]))], xy[1])
                | setColumn(0, move_table.flip_map[__flipMapKey(getColumn(player, xy[0]), getColumn(opponent, xy[0]), getColumn(move_board,xy[0]))], xy[0])
                | setDiagonal(0, move_table.flip_map[__flipMapKey(getDiagonal(player, diag[0]), getDiagonal(opponent, diag[0]), getDiagonal(move_board,diag[0]))], diag[0])
                | setAntiDiagonal(0, move_table.flip_map[__flipMapKey(getAntiDiagonal(player, diag[1]), getAntiDiagonal(opponent, diag[1]), getAntiDiagonal(move_board,diag[1]))], diag[1]);
    if (flip_board == 0) {
        return flip_board;
    }
    else {
        return flip_board | move_board;
    }
    delete[] xy;
    delete[] diag;
}