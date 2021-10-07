#pragma once
#include "game_engine.hpp"

using namespace std;
typedef int (*fptr)(GameEngine*);
MoveEngine * move_engine = &GameEngine::move_engine;

//An evalutation function takes in a board state, and returns some evaluation of the board state
//A more positive evaluation is better for black, a more negative evaluation is better for white
class Evaluation {
    static int eval(GameEngine* engine) {throw "Base evaluation class has no use";}
};

//Evaluation functions
class Random : public Evaluation {
    public:
        //return a random evaluation
        static int eval(GameEngine* engine);
};

class Disks : public Evaluation {
     public:
        //return the no of black disks - no of white disks at a given board state
        static int eval(GameEngine* engine);
};

class Mobility : public Evaluation {
    public:
        //return no of black moves - no of white disks
        static int eval(GameEngine* engine);
};

class MobilityDisks : public Evaluation {
    public:
        //return no of black moves - no of white disks
        static int eval(GameEngine* engine);
};

class Pattern : public Evaluation {
    public:
        //return pattern based evaluation - disks are worth 1, but disks on corner squares are worth 1000
        static int eval(GameEngine* engine);
};

//A search function takes in a board state and evalutaion function, and performs a tree search
//to predict the best move from the current state
class Search {
    public:
        static int search(GameEngine* engine) {throw "Base search class has no use";}
};

//Search functions
class Naive : public Search{
    public:
        //pick the move that results in the best evaluation in the next board state
        static int search(GameEngine* engine, fptr evaluation);
};

class Minimax : public Search{
    public:
        //perform a minimax tree search to a given depth and return the best move
        static int search(GameEngine* engine, fptr evaluation, int depth = 6);
};

class Alphabeta : public Search{
    public:
        //perform a minimax with alpha beta pruning tree search and return the best move
        static int search(GameEngine* engine, fptr evaluation, int depth = 5);
};

class MCTS : public Search{
    public:
        //perform a Monte Carlo Tree search to a given depth and no of simulations, then return the best move
        static int search(GameEngine* engine, fptr evaluation, int depth = 5, int sims = 1000);
};

//A strategy takes in a board state (Game Engine), and returns a valid move to play
//S denotes the search algorithm, E denotes the evaluation function
template <class S, class E>
class StrategyEngine {  
    public:
        //return the evaluation of the board state currently in engine
        static int evaluate(GameEngine* engine) {
            return E::eval(engine);
        }
        //return the best move returned by the search using the given evaluation function
        static int move(GameEngine* engine) {
            return S::search(engine, &E::eval);
        }

};