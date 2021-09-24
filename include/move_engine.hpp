#pragma once
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;
class MoveEngine { 
    public:
        MoveEngine();
        uint64_t getFlipBoard(uint64_t player, uint64_t opponent, int move);
        vector<int> getBitList(uint64_t move_board);
        uint64_t getMoveBoard(uint64_t player, uint64_t opponent);

    private:
        uint64_t __flipMapKey(int player, int opponent, int move);
        void __computeMoves(unsigned int player, unsigned int opponent);
        void __generateMoveTable();
        int __moveArrayIndex(int player, int opponent);
        uint64_t __getHMoves(uint64_t player, uint64_t opponent);
        uint64_t __getVMoves(uint64_t player, uint64_t opponent);
        uint64_t __getDMoves(uint64_t player, uint64_t opponent);
        uint64_t __getADMoves(uint64_t player, uint64_t opponent);
        int * __getXYIndex(int bit);
        int * __getDiagonalIndex(int * xy);
        struct MoveTable {
            char moves[256*256];
            unordered_map<int, char> flip_map;
        } move_table;
};