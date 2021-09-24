//clang++ -O3 -std=c++20 -Iinclude  lib/libothello.so -o bin/othello-text example/othello.cpp

#include <game_engine.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    GameEngine game = GameEngine();
    while (!game.gameEnded()) {
        game.print();
        vector<int> valid_moves = game.getMoveList();
        for (int i : valid_moves) {cout << i << " ";}
        cout << endl;

        cout << "Black to move: "<< endl;

        int move;
        if(cin >> move) {
            while (game.playMove(move) == -3) {
                cout << "Not a valid move";
                cin >> move;
            }
        }
        cin.clear();
        game.print();

        valid_moves = game.getMoveList();
        for (int i : valid_moves) {cout << i << " ";}
        cout << endl;

        cout << "White to move: "<< endl;
        if(cin >> move) {
            while (game.playMove(move) == -3) {
                cout << "Not a valid move";
                cin >> move;
            }
        }
        cin.clear();
        
    }
    cout << "Final score: Black = " << game.getScore() << endl;

}