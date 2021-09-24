#include <python3.9/pyconfig.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "move_engine.hpp"
#include "game_engine.hpp"

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
        .def("reset", &GameEngine::reset)
        .def("gameEnded", &GameEngine::gameEnded)
    ;

    class_<std::vector<int> >("IntVec")
        .def(vector_indexing_suite<std::vector<int> >())
    ;
}