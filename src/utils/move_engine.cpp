#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
#include <bit>
#include "bitboard_ops.hpp"
#include "move_engine.hpp"

using namespace std;

MoveEngine::MoveEngine() {
    generateMoveTable();
}

//a function that returns a key into the flip_map given a black row, a white row and a move
uint64_t MoveEngine::flipMapKey(int player, int opponent, int move) {
        return (player&0xff) << 16 | (opponent&0xff) << 8 | (move&0xff);
}

//precomputes the possible valid moves and their corresponding flips for any row state given by player and opponent
void MoveEngine::computeMoves(unsigned int player, unsigned int opponent) {
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
                    move_table.flip_map[flipMapKey(pl.to_ulong(), op.to_ulong(), (0b1 << j))] |= flipped.to_ulong();
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
                    move_table.flip_map[flipMapKey(pl.to_ulong(), op.to_ulong(), (0b1 << j))] |= flipped.to_ulong();
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
void MoveEngine::generateMoveTable() {
    //generate each possible combination of black and white counters in a row of 8, then compute the moves for this combination
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if ((i & j) == 0) {
               computeMoves(i,j);
            }
        }
    }
}

//generates an index into the move table
int MoveEngine::moveArrayIndex(int player, int opponent){return (player << 8) | opponent ;}

//gets all possible moves on the board in the hoirizontal direction
uint64_t MoveEngine::getHMoves(uint64_t player, uint64_t opponent){
    //generate move for player given the board states stored in player and opponent
    uint64_t moves = 0;
    for (int i = 0; i < 8; i++) {
        moves |= ((uint64_t)move_table.moves[moveArrayIndex(((player >> 8*i) & 0xff), ((opponent >> 8*i) & 0xff))] & 0xff) << i*8;
    }
    return moves & (~(player | opponent));
}

//gets all possible moves on the board in the vertical direction
uint64_t MoveEngine::getVMoves(uint64_t player, uint64_t opponent){
    return rotAC90(getHMoves(rotC90(player), rotC90(opponent)));
}

//gets all possible moves on the board in the direction of the leading diagonal
uint64_t MoveEngine::getDMoves(uint64_t player, uint64_t opponent) {
    uint64_t rplayer = rotAC45(player);
    uint64_t ropponent = rotAC45(opponent); 
    unsigned int shift[11] = {32,  24,   16,   8,    0,    56,   48,   40,   32,   24,   16};
    unsigned int mask[11] = {0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};
    uint64_t moves = 0;
    for (int i = 0; i < 11; i++ ) {
            unsigned int m = mask[i];
            unsigned int s = shift[i];
            moves |= (((uint64_t)move_table.moves[moveArrayIndex((rplayer >> s) & m, (ropponent >> s)   & m)] & m) << s);
            }
    return rotC45(moves) & (~(player | opponent));
}

//gets all the possible moves on the board in the direction of the anti-diagonal
uint64_t MoveEngine::getADMoves(uint64_t player, uint64_t opponent) {
    return flipV(getDMoves(flipV(player), flipV(opponent)));
}

//gets all the possible moves on the board
uint64_t MoveEngine::getMoveBoard(uint64_t player, uint64_t opponent) {
    return  getVMoves(player, opponent) | getHMoves(player, opponent) | getDMoves(player, opponent) | getADMoves(player, opponent);
}

//a function that decomposes a bit board into a set of integers representing the position of 
//each bit on the bitboard
set<int> MoveEngine::getBitList(uint64_t move_board) {
    set<int> move_indices;
     while (move_board != 0){
        uint64_t current_move = __bit_floor(move_board);
        move_board = move_board - current_move;
        move_indices.insert(log2(current_move));
    } 
    return move_indices;
}

//a function that takes the bit of a bitboard and returns the x,y index MSB (0,0) 
pair<int,int> MoveEngine::getXYIndex(int bit) {
    return pair<int,int>((63-bit)%8, (63-bit)/8);
}

//a function that takes an (x,y) pair and returns a pair of (diagonal, antidiagonal) indices.
pair<int,int> MoveEngine::getDiagonalIndex(pair<int, int> xy) {
    return pair<int,int> ((5 + xy.second - xy.first), (-2 + xy.second + xy.first));
}


//a function that returns a bitboard of opponents pieces that are flipped on a move. player and opponent are the current bitboards, move is an integer value representing the number of the bit that the move is on, flip mop is the map of all moves
uint64_t MoveEngine::getFlipBoard(uint64_t player, uint64_t opponent, int move){
    // get x, y, d and ad
    pair<int,int> xy = getXYIndex(move);
    pair<int,int> diag = getDiagonalIndex(xy);
    uint64_t move_board = (uint64_t)0b1 << move;
    //set row
    uint64_t flip_board  = setRow(0, move_table.flip_map[flipMapKey(getRow(player, xy.second), getRow(opponent, xy.second), getRow(move_board, xy.second))], xy.second)
                | setColumn(0, move_table.flip_map[flipMapKey(getColumn(player, xy.first), getColumn(opponent, xy.first), getColumn(move_board,xy.first))], xy.first)
                | setDiagonal(0, move_table.flip_map[flipMapKey(getDiagonal(player, diag.first), getDiagonal(opponent, diag.first), getDiagonal(move_board,diag.first))], diag.first)
                | setAntiDiagonal(0, move_table.flip_map[flipMapKey(getAntiDiagonal(player, diag.second), getAntiDiagonal(opponent, diag.second), getAntiDiagonal(move_board,diag.second))], diag.second);
    if (flip_board == 0) {
        return flip_board;
    }
    else {
        return flip_board | move_board;
    }
        
}