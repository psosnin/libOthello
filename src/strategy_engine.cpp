#include <vector>
#include <iostream>
#include <functional>
#include "strategy_engine.hpp"
#include "game_engine.hpp"
#include <cassert>

//evaluation functions
using namespace std;

//random no. generator:
static unsigned long x=123456789, y=362436069, z=521288629;
int positions;

unsigned long xorshf96() {
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z;
}

class Node {
    public:
        //return a new node with the given black and white bitboard state
        Node(uint64_t b, uint64_t w, int p) {
            black = b;
            white = w;
            player = p;
            __getMoves();
        }
        //return a new node that is the result of the given move played from the parent node
        Node(Node* parent, int move) {
            black = parent->black;
            white = parent->white;
            player = parent->player;
            if (parent->moves.empty()) {
                //pass the move
                player *= -1;
            } else if (player == 1) {
                move_engine->playMove(&black, &white, move);
            } else {
                move_engine->playMove(&white, &black, move);
            }
            __getMoves();
        }
        Node* parent;
        vector<int> moves;
        vector<Node*> children;
        uint64_t black;
        uint64_t white;
        int player; //1 = black to move, -1 = white to move
        int evaluation; // +ve better for black, -ve better for white
        void addChild(int move);

    private:
        vector<int> __getMoves() {
            if (player == 1) {
                moves = move_engine->getBitList(move_engine->getMoveBoard(black, white));
            } else {
                moves = move_engine->getBitList(move_engine->getMoveBoard(white, black));
            }
            return moves;
        }
};

int Random::eval(GameEngine* engine) {
    return xorshf96() % 10;
}

int Disks::eval(GameEngine* engine) {
    return engine->getBlackList().size() - engine->getWhiteList().size();
}

int Mobility::eval(GameEngine* engine) {
    int black = move_engine->getNoBits(move_engine->getMoveBoard(engine->getBlackBoard(),
                                               engine->getWhiteBoard()));
    int white = move_engine->getNoBits(move_engine->getMoveBoard(engine->getWhiteBoard(),
                                               engine->getBlackBoard()));
    return black - white;
}

int MobilityDisks::eval(GameEngine* engine) {
    return (63 - engine->getMoveCount())*Mobility::eval(engine) + engine->getMoveCount()*Disks::eval(engine);
}

//return the number of corners set in the given bit board
int corners(uint64_t board) {
    uint64_t corners = board & 0x8100000000000081;
    return move_engine->getNoBits(corners);
}

int Pattern::eval(GameEngine* engine) {
    int score = 0;
    score += Disks::eval(engine);
    score += 10*Mobility::eval(engine);
    score += 1000*(corners(engine->getBlackBoard()) - corners(engine->getWhiteBoard()));
    return score;
}

//search functions
int Naive::search(GameEngine* engine, fptr evaluation) {
    vector<int> moves = engine->getMoveList();
    int max_eval = -10000;
    int best_move;
    int player = engine->getPlayer();
    for (int i : moves) {
        engine->playMove(i);
        int eval = player*evaluation(engine);
        if (eval > max_eval) {
            best_move = i;
            max_eval = eval;
        }
        //cout << "player: " << player << " move: " << i << " eval: " << eval << endl;
        engine->undoLastMove();
    }
    //cout << " best move: " << best_move << endl;
    return best_move;
}

int minimax(GameEngine* engine, fptr evaluation, int depth) {
    if (depth == 0) {
        positions++;
        return evaluation(engine);
    } else if (engine->gameEnded()) {
        if (engine->getScore() == 32) {
            return 0;
        } else {}
            return engine->getScore() > 32 ? 10000 : -10000;
    }

    vector<int> moves = engine->getMoveList();
    if (moves.size() == 0) { //pass turn
        engine->playMove(-1);
        int eval = minimax(engine, evaluation, depth - 1);
        engine->undoLastMove();
        return eval;
    } else if (engine->getPlayer() == 1) { //maximising player
        int eval = -10000000;
        for (int i : moves) {
            engine->playMove(i);
            eval = max(eval, minimax(engine, evaluation, depth - 1));
            engine->undoLastMove();
        }
        return eval;
    } else { //minimising player
        int eval = 10000000;
        for (int i : moves) {
            engine->playMove(i);
            eval = min(eval, minimax(engine, evaluation, depth - 1));
            engine->undoLastMove();
        }
        return eval;
    }
}

int Minimax::search(GameEngine* engine, fptr evaluation, int depth) {
    positions = 0;
    vector<int> moves = engine->getMoveList();
    if (moves.size() == 0) {
        return -1;
    }
    if (engine->getPlayer() == 1) { //maximising player
        int max_eval = -10000000;
        int best_move = -1;
        for (int i : moves) {
            engine->playMove(i);
            int eval = minimax(engine, evaluation, depth - 1);
            if (max_eval < eval) {
                max_eval = eval;
                best_move = i;
            }
            engine->undoLastMove();
        }
        cout << positions << " positions searched" << endl;
        return best_move;
    } else {
        int min_eval = 10000000;
        int best_move = -1;
        for (int i : moves) {
            engine->playMove(i);
            int eval = minimax(engine, evaluation, depth - 1);
            if (min_eval > eval) {
                min_eval = eval;
                best_move = i;
            }
            engine->undoLastMove();
        }
        cout << positions << " positions searched" << endl;
        return best_move;
    }
}

int Alphabeta::search(GameEngine* engine, fptr evaluation, int depth) {
    throw "not implemented";
}

int MCTS::search(GameEngine* engine, fptr evaluation, int depth, int sims) {
    throw "not implemented";
}