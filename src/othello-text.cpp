#include <game_engine.hpp>
#include <iostream>
#include <set>

using namespace std;

int main() {
    GameEngine game = GameEngine();
    while (!game.gameEnded()) {
        game.print();
        set<int> valid_moves = game.getMoveList();
        for (int i : valid_moves) {cout << i << " ";}
        cout << endl;

        cout << "Black to move: "<< endl;

        int move;
        if(cin >> move) {
            while (game.playMove(move) == -3) {
                cout << "Not a valid move";
                cin >> move;
            }
            set<int> changed = game.getBoardChange(1);
            for (auto i : changed) {
                cout << "changed = " << i << endl;
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
            set<int> changed = game.getBoardChange(-1);
            for (auto i : changed) {
                cout << "changed = " << i << endl;
            }
        }
        cin.clear();
        
    }
    cout << "Final score: Black = " << game.getScore() << endl;

}