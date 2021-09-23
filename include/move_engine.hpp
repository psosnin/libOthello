#pragma once
#include <unordered_map>
#include <set>
#include <string>

using namespace std;
class MoveEngine { 
    public:
        MoveEngine();
        uint64_t getFlipBoard(uint64_t player, uint64_t opponent, int move);
        set<int> getBitList(uint64_t move_board);
        uint64_t getMoveBoard(uint64_t player, uint64_t opponent);

    private:
        uint64_t flipMapKey(int player, int opponent, int move);
        void computeMoves(unsigned int player, unsigned int opponent);
        void generateMoveTable();
        int moveArrayIndex(int player, int opponent);
        uint64_t getHMoves(uint64_t player, uint64_t opponent);
        uint64_t getVMoves(uint64_t player, uint64_t opponent);
        uint64_t getDMoves(uint64_t player, uint64_t opponent);
        uint64_t getADMoves(uint64_t player, uint64_t opponent);
        pair<int,int> getXYIndex(int bit);
        pair<int,int> getDiagonalIndex(pair<int, int> xy);
        struct MoveTable {
            char moves[256*256];
            unordered_map<int, char> flip_map;
        } move_table;
};