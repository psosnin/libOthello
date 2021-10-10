#include <python3.9/pyconfig.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "move_engine.hpp"
#include "game_engine.hpp"
#include "strategy_engine.hpp"

// define boost.python module
BOOST_PYTHON_MODULE(pythello)
{
    using namespace boost::python;
    class_<MoveEngine>("MoveEngine")
        .def("getFlipBoard", &MoveEngine::getFlipBoard)
        .def("getMoveBoard", &MoveEngine::getMoveBoard)
        .def("getBitList", &MoveEngine::getBitList)
    ;

    class_<GameEngine>("GameEngine")
        .def("print", &GameEngine::print)
        .def("getMoveList", &GameEngine::getMoveList)
        .def("playMove", &GameEngine::playMove)
        .def("getPlayer", &GameEngine::getPlayer)
        .def("getBoardChange", &GameEngine::getBoardChange)
        .def("getScore", &GameEngine::getScore)
        .def("getWhiteScore", &GameEngine::getWhiteScore)
        .def("reset", &GameEngine::reset)
        .def("gameEnded", &GameEngine::gameEnded)
        .def("undoLastMove", &GameEngine::undoLastMove)
        .def("redoNextMove", &GameEngine::redoNextMove)
        .def("getBlackList", &GameEngine::getBlackList)
        .def("getWhiteList", &GameEngine::getWhiteList)
        .def("getBlackBoard", &GameEngine::getBlackBoard)
        .def("getWhiteBoard", &GameEngine::getWhiteBoard)
        .def("getMoveCount", &GameEngine::getMoveCount)
    ;

    class_<std::vector<int> >("IntVec")
        .def(vector_indexing_suite<std::vector<int> >())
        .def("size", &vector_indexing_suite<std::vector<int> >::size)
    ;

    class_<StrategyEngine<Naive, Random>>("NaiveRandom")
        .def("evaluate", &StrategyEngine<Naive, Random>::evaluate)
        .def("move", &StrategyEngine<Naive, Random>::move)
        ;
    
    class_<StrategyEngine<Naive, Disks>>("NaiveDisks")
        .def("evaluate", &StrategyEngine<Naive, Disks>::evaluate)
        .def("move", &StrategyEngine<Naive, Disks>::move)
        ;
    
    class_<StrategyEngine<Naive, Mobility>>("NaiveMobility")
        .def("evaluate", &StrategyEngine<Naive, Mobility>::evaluate)
        .def("move", &StrategyEngine<Naive, Mobility>::move)
        ;

    class_<StrategyEngine<Minimax, Random>>("MinimaxRandom")
        .def("evaluate", &StrategyEngine<Minimax, Random>::evaluate)
        .def("move", &StrategyEngine<Minimax, Random>::move)
        ;

    class_<StrategyEngine<Minimax, Disks>>("MinimaxDisks")
        .def("evaluate", &StrategyEngine<Minimax, Disks>::evaluate)
        .def("move", &StrategyEngine<Minimax, Disks>::move)
        ;

    class_<StrategyEngine<Minimax, Mobility>>("MinimaxMobility")
        .def("evaluate", &StrategyEngine<Minimax, Mobility>::evaluate)
        .def("move", &StrategyEngine<Minimax, Mobility>::move)
        ;

    class_<StrategyEngine<Minimax, Pattern>>("MinimaxPattern")
        .def("evaluate", &StrategyEngine<Minimax, Pattern>::evaluate)
        .def("move", &StrategyEngine<Minimax, Pattern>::move)
        ;

    class_<StrategyEngine<Alphabeta, Pattern>>("AlphabetaPattern")
        .def("evaluate", &StrategyEngine<Alphabeta, Pattern>::evaluate)
        .def("move", &StrategyEngine<Alphabeta, Pattern>::move)
        ;

    class_<StrategyEngine<AlphabetaFast, Pattern>>("AlphabetaFast")
        .def("evaluate", &StrategyEngine<AlphabetaFast, Pattern>::evaluate)
        .def("move", &StrategyEngine<AlphabetaFast, Pattern>::move)
        ;


    class_<StrategyEngine<MCTS, Pattern>>("MCTS")
        .def("evaluate", &StrategyEngine<MCTS, Pattern>::evaluate)
        .def("move", &StrategyEngine<MCTS, Pattern>::move)
        ;
}