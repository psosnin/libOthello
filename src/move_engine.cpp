#include <bitset>
#include <unordered_map>
#include <cmath>
#include <set>
#include <bit>
#include <iostream>
#include "bitboard_ops.hpp"
#include "move_engine.hpp"

using namespace std;

MoveEngine::MoveEngine() {
    __generateMoveTable();
}

uint64_t MoveEngine::__flipMapKey(int player, int opponent, int move) {
        return (player&0xff) << 16 | (opponent&0xff) << 8 | (move&0xff);
}

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

void MoveEngine::__generateMoveTable() {
    cout << "Computing moves, should only happen once" << endl;
    //generate each possible combination of black and white counters in a row of 8, then compute the moves for this combination
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if ((i & j) == 0) {
               __computeMoves(i,j);
            }
        }
    }
}

int MoveEngine::__moveArrayIndex(int player, int opponent){return (player << 8) | opponent ;}

uint64_t MoveEngine::__getHMoves(uint64_t player, uint64_t opponent){
    //generate move for player given the board states stored in player and opponent
    uint64_t moves = 0;
    for (int i = 0; i < 8; i++) {
        moves |= ((uint64_t)move_table.moves[__moveArrayIndex(((player >> 8*i) & 0xff), ((opponent >> 8*i) & 0xff))] & 0xff) << i*8;
    }
    return moves & (~(player | opponent));
}

uint64_t MoveEngine::__getVMoves(uint64_t player, uint64_t opponent){
    return rotAC90(__getHMoves(rotC90(player), rotC90(opponent)));
}

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

uint64_t MoveEngine::__getADMoves(uint64_t player, uint64_t opponent) {
    return flipV(__getDMoves(flipV(player), flipV(opponent)));
}

uint64_t MoveEngine::getMoveBoard(uint64_t player, uint64_t opponent) {
    return __getVMoves(player, opponent) | __getHMoves(player, opponent) | __getDMoves(player, opponent) | __getADMoves(player, opponent);
}

vector<int> MoveEngine::getBitList(uint64_t move_board) {
    vector<int> move_indices;
     while (move_board != 0){
        uint64_t current_move = __bit_floor(move_board);
        move_board = move_board - current_move;
        move_indices.push_back(log2(current_move));
    } 
    return move_indices;
}

void MoveEngine::__getXYIndex(int bit, int * xy) {
    xy[0] = (63-bit)%8;
    xy[1] = (63-bit)/8;
}

void MoveEngine::__getDiagonalIndex(int * xy, int * diag) {
    diag[0] = 5 + xy[1] - xy[0];
    diag[1] =  -2 + xy[1] + xy[0];
}

uint64_t MoveEngine::getFlipBoard(uint64_t player, uint64_t opponent, int move){
    // get x, y, d and ad
    int xy[2];
    __getXYIndex(move, xy);
    int diag[2];
    __getDiagonalIndex(xy, diag);
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
}

void MoveEngine::playMove(uint64_t * player, uint64_t * opponent, int move) {
    uint64_t flips = getFlipBoard(*player, *opponent, move);
    *opponent &= ~flips;
    *player |= flips;
}

int MoveEngine::getNoBits(uint64_t board) {
    int bits = 0;
    while (board != 0){
        board = board - __bit_floor(board);
        bits++;
    }
    return bits;
}