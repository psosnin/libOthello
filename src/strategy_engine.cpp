#include <vector>
#include <iostream>
#include <functional>
#include <limits>
#include "strategy_engine.hpp"
#include "game_engine.hpp"
#include <cassert>
#include <cmath>
#include "bitboard_ops.hpp"

//evaluation functions
using namespace std;

//random no. generator:
static unsigned long x=122345782, y=362436069, z=521288629;
int positions;
int nodes;
int rollouts;

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
    int max_eval = numeric_limits<int>::min();
    int best_move;
    int player = engine->getPlayer();
    for (int i : moves) {
        engine->playMove(i);
        int eval = player*evaluation(engine);
        if (eval > max_eval) {
            best_move = i;
            max_eval = eval;
        }
        engine->undoLastMove();
    }
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
        int eval = numeric_limits<int>::min();
        for (int i : moves) {
            engine->playMove(i);
            eval = max(eval, minimax(engine, evaluation, depth - 1));
            engine->undoLastMove();
        }
        return eval;
    } else { //minimising player
        int eval = numeric_limits<int>::max();
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
        int max_eval = numeric_limits<int>::min();
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
        int min_eval = numeric_limits<int>::max();
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

int alphabeta(GameEngine* engine, fptr evaluation, int alpha, int beta, int depth) {
    //alpha = highest evaluation we have encountered
    //beta = lowest evaluation we have encountered
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
        int eval = alphabeta(engine, evaluation, alpha, beta, depth - 1);
        engine->undoLastMove();
        return eval;
    } else if (engine->getPlayer() == 1) { //maximising player
        int eval = numeric_limits<int>::min();
        for (int i : moves) {
            engine->playMove(i);
            eval = max(eval, alphabeta(engine, evaluation, alpha, beta, depth - 1));
            engine->undoLastMove();
            alpha = max(alpha, eval);
            if (beta<=alpha) break;
        }
        return eval;
    } else { //minimising player
        int eval = numeric_limits<int>::max();
        for (int i : moves) {
            engine->playMove(i);
            eval = min(eval, alphabeta(engine, evaluation, alpha, beta, depth - 1));
            engine->undoLastMove();
            beta = min(beta, eval); 
            if (beta<=alpha) break;
        }
        return eval;
    }
}

int Alphabeta::search(GameEngine* engine, fptr evaluation, int depth) {
    positions = 0;
    vector<int> moves = engine->getMoveList();
    if (moves.size() == 0) {
        return -1;
    }
    if (engine->getPlayer() == 1) { //maximising player
        int max_eval = numeric_limits<int>::min();
        int best_move = -1;
        for (int i : moves) {
            engine->playMove(i);
            int eval = alphabeta(engine, evaluation, numeric_limits<int>::min(), 
                                 numeric_limits<int>::max(), depth - 1);
            if (max_eval < eval) {
                max_eval = eval;
                best_move = i;
            }
            engine->undoLastMove();
        }
        cout << positions << " positions searched" << endl;
        return best_move;
    } else {
        int min_eval = numeric_limits<int>::max();
        int best_move = -1;
        for (int i : moves) {
            engine->playMove(i);
            int eval = alphabeta(engine, evaluation, numeric_limits<int>::min(), 
                                 numeric_limits<int>::max(), depth - 1);
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

int AlphabetaFast::search(GameEngine* engine, fptr evaluation, int depth) {
    positions = 0;
    vector<int> moves = engine->getMoveList();
    if (moves.size() == 0) {
        return -1;
    }
    if (engine->getPlayer() == 1) { //maximising player
        int max_eval = numeric_limits<int>::min();
        int best_move = -1;
        for (int i : moves) {
            GameEngine * thread_engine { engine };
            thread_engine->playMove(i);
            int eval = alphabeta(thread_engine, evaluation, numeric_limits<int>::min(), 
                                 numeric_limits<int>::max(), depth - 1);
            if (max_eval < eval) {
                max_eval = eval;
                best_move = i;
            }
            thread_engine->undoLastMove();
        }
        cout << positions << " positions searched" << endl;
        return best_move;
    } else {
        int min_eval = numeric_limits<int>::max();
        int best_move = -1;
        for (int i : moves) {
            GameEngine * thread_engine { engine };
            thread_engine->playMove(i);
            int eval = alphabeta(thread_engine, evaluation, numeric_limits<int>::min(), 
                                 numeric_limits<int>::max(), depth - 1);
            if (min_eval > eval) {
                min_eval = eval;
                best_move = i;
            }
            thread_engine->undoLastMove();
        }
        cout << positions << " positions searched" << endl;
        return best_move;
    }
}



class Node {
    public:
        //return a new node with the given black and white bitboard state
        Node(uint64_t b, uint64_t w, int p) :
            wins(0), visits(0) {
            nodes++;
            black = b;
            white = w;
            player = p;
            __getMoves();
        }
        //return a new node that is the result of the given move played from the parent node
        Node(Node* p, int move) :
            wins(0), visits(0) {
            nodes++;
            parent = p;
            black = p->black;
            white = p->white;
            player = p->player*-1;
            if (p->moves.empty()) {
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
        int wins;
        int visits;
        int player; //1 = black to move, -1 = white to move
        int evaluation; // +ve better for black, -ve better for white
        void addChildren() {
            if (moves.size() == 0) {
                Node * child = new Node(this, -1);
                children.push_back(child);
            } else {
                for (int i : moves) {
                    Node * child = new Node(this, i);
                    children.push_back(child);
                }
            }
        }
        void printTree(int l = 0) {
            if (this->visits != 0) {
                for (int i  = 0; i < l; i++) {
                    cout << " ";
                }
                cout << visits << " | " << wins << endl;
                for (Node * child : children) {
                    child->printTree(l+1);
                }
            }
        }
        ~Node() {
            for (auto child : children) {
                delete child;
            }
        }

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

float UCB(Node * node) {
    if (node->visits == 0) {
        return INFINITY;
    } else if (node->parent == nullptr) {
        return ((float)node->wins / (float)node->visits);
    } else {
        return ((float)node->wins / (float)node->visits) + sqrt(log2(node->parent->visits) / node->visits);
    }
}

float averageScore(Node * node) {
    cout << "Child has  " << node->visits << " visits and " << node->wins << " wins" << endl;
    if (node->visits == 0) return 0;
    return (float) node->wins / (float) node->visits;
}

//perform a fast random rollout from the given node
int randomRollout(Node * node) {
    rollouts++;
    uint64_t white = node->white;
    uint64_t black = node->black;
    int player = node->player;
    vector<int> moves;
    int count = 0; //no of moves that have been passed
    while (count < 2) {
        if (player == 1) {
            player *= -1;
            moves = move_engine->getMoveList(black, white);
            if (moves.size() == 0) {
                count++;
            } else {
                int i = xorshf96() % moves.size();
                move_engine->playMove(&black, &white, moves[i]);
                count = 0;
            }
        } else {
            moves = move_engine->getMoveList(white, black);
            player *= -1;
            if (moves.size() == 0) {
                count++;
            } else {
                int i = xorshf96() % moves.size();
                move_engine->playMove(&white, &black, moves[i]);
                count = 0;
            }
        }
    }
    int score = move_engine->getNoBits(black) - move_engine->getNoBits(white);
    //cout << score << endl;
    //if (score == 0) return 0;
    return score > 0 ? 1 : 0;
}

//mcts: if lead node, create new child node, run a random rollout from the child node, the backpropagate the results
//if not a leaf node, traverse the tree following the child node with the highest UCB
int mcts(Node * node) {
    int result = 0;
    node->visits++;
    if (node->moves.empty() && node->parent->moves.empty()) {
        //terminal node, return 1 for win, 0 for draw and -1 for loss
        int score = move_engine->getNoBits(node->black) - move_engine->getNoBits(node->white);
        //if (score == 0) return 0;
        return score > 0 ? 1 : 0;
    } else if (node->children.empty()) {
        //leaf node, add all the child nodes and run a random rollout from this node
        node->addChildren();
        result = randomRollout(node);
    } else {
        //traverse down the tree according to UCB
        float max_ucb = -INFINITY, best_child = 0, ucb;
        for (int i = 0; i < node->children.size(); i++) {
            Node * child = node->children[i];
            ucb = UCB(child);
            //cout << ucb << endl;
            if (ucb > max_ucb) {
                max_ucb = ucb;
                best_child = i;
            }
        }
        result = mcts(node->children[best_child]);
    }
    node->wins += result;
    return result;
}

//only works as black for now, segfaults on pass moves
int MCTS::search(GameEngine* engine, fptr evaluation, int sims) {
    nodes = 0;
    rollouts = 0;
    Node root = Node(engine->getBlackBoard(), engine->getWhiteBoard(), engine->getPlayer());
    if (engine->getNoMoves() == 0) {
        //pass turn
        return -1;
    }
    //run sims monte carlo simulations
    for (int i = 0; i < sims; i++) {
        mcts(&root);
    }
    int best_move;
    if (engine->getPlayer() == 1) {
        //return the child with the highest average score
        float max_score = -INFINITY, score;
        for (int i = 0; i < root.moves.size(); i++) {
            score = averageScore(root.children[i]);
            if (score > max_score) {
                max_score = score;
                best_move = i;
            }
        }
    } else {
        //return the child with the lowest average score
        float min_score = INFINITY, score;
        for (int i = 0; i < root.moves.size(); i++) {
            score = averageScore(root.children[i]);
            if (score < min_score) {
                min_score = score;
                best_move = i;
            }
        }
    }
    cout << "Nodes created: " << nodes << endl;
    cout << "Rollouts performed: " << rollouts << endl;
    //root.printTree();
    cout << "Choosing node with " << root.children[best_move]->visits << " visits and " << root.children[best_move]->wins << " wins" << endl;
    return root.moves[best_move];
}