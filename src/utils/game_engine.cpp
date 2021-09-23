#include "bitboard_ops.hpp"
#include "move_engine.hpp"
#include "game_engine.hpp"
#include <bitset>
#include <set>
#include <iostream>

using namespace std;

MoveEngine GameEngine::move_engine = MoveEngine();

//initialises the starting game state
GameEngine::GameEngine() {
        player1 = 0x0000000810000000; //starting board for black
        player2 = 0x0000001008000000; //starting board for white
        past1 = 0x0000000810000000; //starting board for black
        past2 = 0x0000001008000000; //starting board for white
        move_count = 0;    
        updateMoveBoard();
        game_ended = false;
    }

//carries out the logic when the move passed in is requested and returns the results:
//   -3 if the move is invalid
//   -2 if the game has ended
//   -1 if there are no valid moves, and we pass the turn
//   otherwise, return the move index
int GameEngine::playMove(int move) {
    if (game_ended) {
        return -2;
    } else if (current_moves == 0) {
        move_count++;
        updateMoveBoard();
        return -1;
    } else if ((((unsigned long)0b1 << move) & current_moves) != 0) {
        flip(move);
        move_count++;
        updateMoveBoard();
        return move;
    } else {
        return -3;
    }
}

//updates the current valid moves given the current board states
void GameEngine::updateMoveBoard() {
    if (move_count % 2 == 0) {
        current_moves = move_engine.getMoveBoard(player1, player2);
        if (current_moves == 0 && move_engine.getMoveBoard(player2, player1) == 0) {
            game_ended = true;
        }
    } else {
        current_moves = move_engine.getMoveBoard(player2, player1);
        if (current_moves == 0 && move_engine.getMoveBoard(player1, player2) == 0) {
            game_ended = true;
        }
    }
    
}

//gets a list of all the possible moves in the current board state
set<int> GameEngine::getMoveList(bool t) {
    if (t) {updateMoveBoard();}
    return move_engine.getBitList(current_moves);
}

//flips all of the pieces neccessary when a move is taken
void GameEngine::flip(int move) {
    past1 = player1;
    past2 = player2;
    if (move_count % 2 == 0) {
        uint64_t flips = move_engine.getFlipBoard(player1, player2, move);
        player2 &= ~flips;
        player1 |= flips;
    } else {
        uint64_t flips = move_engine.getFlipBoard(player2, player1, move);
        player1 &= ~flips;
        player2 |= flips;
    }
}

//prints the board state to the command line
void GameEngine::print() {
    pprintBoard(player1, player2, current_moves);
}

//returns the player whos move it is 
int GameEngine::getPlayer() {
    //1 for black, -1 for white
    return move_count % 2 == 0;
}

//returns a set of all the pieces that were flipped or placed on the previous turn
set<int> GameEngine::getBoardChange(int player) {
    if (player == 1) {
        return move_engine.getBitList(player1 & ~past1);
    } else {
        return move_engine.getBitList(player2 & ~past2);
    }
}

//returns the number of counters black has. If the game has ended, then any empty spaces are given to the winner.
int GameEngine::getScore() {
    int black = (bitset<64> (player1)).count();
    int white = (bitset<64> (player2)).count();
    int empty = 64 - white - black;
    if (!game_ended) {
        return black;
    } else if (black > white) {
        return black + empty;
    } else if (white == black) {
        return 32;
    }
    return black;
}

//resets the game
void GameEngine::reset() {
    player1 = 0x0000000810000000; //starting board for black
    player2 = 0x0000001008000000; //starting board for white
    past1 = 0x0000000810000000; //starting board for black
    past2 = 0x0000001008000000; //starting board for white
    move_count = 0;
    updateMoveBoard();
    game_ended = false;
}

bool GameEngine::gameEnded() {
    return game_ended;
}