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
        board1[0] = 0x0000000810000000;
        board1[1] = 0x0000000810000000;
        board2[0] = 0x0000001008000000;
        board2[1] = 0x0000001008000000;
        player = 1; //1 for black, -1 for white
        count = 1;
        undone = 0;
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
        player *= -1;
        __updateMoveBoard();
        return -1;
    } else if ((((unsigned long)0b1 << move) & current_moves) != 0) {
        __flip(move);
        player *= -1;
        undone = 0;
        count++;
        __updateMoveBoard();
        return move;
    } else {
        return -3;
    }
}

//updates the current valid moves given the current board states
void GameEngine::__updateMoveBoard() {
    if (player == 1) {
        current_moves = move_engine.getMoveBoard(board1[count], board2[count]);
        if (current_moves == 0 && move_engine.getMoveBoard(board2[count], board1[count]) == 0) {
            game_ended = true;
        }
    } else {
        current_moves = move_engine.getMoveBoard(board2[count], board1[count]);
        if (current_moves == 0 && move_engine.getMoveBoard(board1[count], board2[count]) == 0) {
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
    if (player == 1) {
        uint64_t flips = move_engine.getFlipBoard(board1[count], board2[count], move);
        board2[count + 1] = board2[count] & ~flips;
        board1[count + 1] = board1[count] | flips;
    } else {
        uint64_t flips = move_engine.getFlipBoard(board2[count], board1[count], move);
        board1[count + 1] = board1[count] & ~flips;
        board2[count + 1] = board2[count] | flips;
    }
}

//prints the board state to the command line
void GameEngine::print() {
    pprintBoard(board1[count], board2[count], current_moves);
}

//returns the player whos move it is 
int GameEngine::getPlayer() {
    //1 for black, -1 for white
    return player;
}

//returns a vector of all the pieces that were flipped or placed on the previous turn
vector<int> GameEngine::getBoardChange(int p) {
    if (p == 1) {
        return move_engine.getBitList(board1[count] & ~board1[count-1]);
    } else {
        return move_engine.getBitList(board2[count] & ~board2[count-1]);
    }
}

//returns the number of counters black has. If the game has ended, then any empty spaces are given to the winner.
int GameEngine::getScore() {
    int black = (bitset<64> (board1[count])).count();
    int white = (bitset<64> (board2[count])).count();
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
    int black = (bitset<64> (board1[count])).count();
    int white = (bitset<64> (board2[count])).count();
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
    count = 1;
    undone = 0;
    player = 1;
    __updateMoveBoard();
    game_ended = false;
}

bool GameEngine::gameEnded() {
    return game_ended;
}

//resets the game engine to the state before the previous move occurred 
void GameEngine::undoLastMove() {
    if (count <= 1) return;
    undone++;
    count--;
    player *= -1;
    game_ended = false;
    __updateMoveBoard();
}

//if there is currently a move that has been undone, replay that move
void GameEngine::redoNextMove() {
    if (undone == 0) return;
    undone--;
    count++;
    player *= -1;
    __updateMoveBoard();
}

//return a vector of all squares that have a black counter
vector<int> GameEngine::getBlackList(){
    return move_engine.getBitList(board1[count]);
}
//return a vector of all squares that have a white counter
vector<int> GameEngine::getWhiteList(){
    return move_engine.getBitList(board2[count]);
}