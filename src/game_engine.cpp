#include "bitboard_ops.hpp"
#include "move_engine.hpp"
#include "game_engine.hpp"
#include <bitset>
#include <vector>
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
        __updateMoveBoard();
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
        __updateMoveBoard();
        return -1;
    } else if ((((unsigned long)0b1 << move) & current_moves) != 0) {
        __flip(move);
        move_count++;
        __updateMoveBoard();
        return move;
    } else {
        return -3;
    }
}

//updates the current valid moves given the current board states
void GameEngine::__updateMoveBoard() {
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
vector<int> GameEngine::__getMoveList(bool t) {
    if (t) {__updateMoveBoard();}
    return move_engine.getBitList(current_moves);
}

//flips all of the pieces neccessary when a move is taken
void GameEngine::__flip(int move) {
    past1 = player1;
    past2 = player2;
    history1[move_count] = past1;
    history2[move_count] = past2;
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
    return (move_count % 2 == 0) ? 1 : -1;
}

//returns a vector of all the pieces that were flipped or placed on the previous turn
vector<int> GameEngine::getBoardChange(int player) {
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

//return the number of counters white has. If the game has ended, then any empty spaces are given to the winner.
int GameEngine::getWhiteScore() {
    int black = (bitset<64> (player1)).count();
    int white = (bitset<64> (player2)).count();
    int empty = 64 - white - black;
    if (!game_ended) {
        return white;
    } else if (white > black) {
        return white + empty;
    } else if (white == black) {
        return 32;
    }
    return white;
}
//resets the game
void GameEngine::reset() {
    player1 = 0x0000000810000000; //starting board for black
    player2 = 0x0000001008000000; //starting board for white
    past1 = 0x0000000810000000; //starting board for black
    past2 = 0x0000001008000000; //starting board for white
    move_count = 0;
    __updateMoveBoard();
    game_ended = false;
}

bool GameEngine::gameEnded() {
    return game_ended;
}

//resets the game engine to the state before the previous move occurred 
void GameEngine::undoLastMove() {
    if (move_count == 0) return;
    player1 = history1[move_count-1];
    player2 = history2[move_count-1];
    past1 = history1[move_count-2];
    past2 = history2[move_count-2];
    move_count--;
    game_ended = false;
    __updateMoveBoard();
}

//return a vector of all squares that have a black counter
vector<int> GameEngine::getBlackList(){
    return move_engine.getBitList(player1);
}
//return a vector of all squares that have a white counter
vector<int> GameEngine::getWhiteList(){
    return move_engine.getBitList(player2);
}