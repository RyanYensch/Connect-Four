#include <iostream>
#include <vector>
using namespace std;

class ConnectFour {
    private:
    vector<vector<char>> arrayOfVectors;  // Declare it without initialization
    char playerChar[2] = {'X', 'O'};

    public:
    // Constructor with member initializer list
    ConnectFour() : arrayOfVectors(7) {}

    private:
    void clearScreen() {
        cout << "\033[2J\033[H";
    }

    void printBoard() {
        for (int j = 5; j >= 0; j--) {
            for (int i = 0; i < 7; i++) {
                if (arrayOfVectors[i].size() > j) {
                    cout << '|' << arrayOfVectors[i].at(j); 
                } else {
                    cout << "| ";
                }
            }
            cout << "|\n";
        }
    }

    void playerMove(int playerID, int col) {
        arrayOfVectors[col].push_back(playerChar[playerID]);
    }

    bool checkWin(int col) {
        return false;
    }

    public:
    void playGame() {
        int col;
        int turn = 0;
        do {
            clearScreen();
            printBoard();
            do {
                cout << "Which Column (0-6): ";
                cin >> col;
            } while ((col < 0 || col >= 7) || arrayOfVectors[col].size() >= 6);

            playerMove(turn, col);
            turn = (turn + 1) % 2;

        } while(checkWin(col) == false);
    }
};

int main() {
    ConnectFour game;

    game.playGame();
    return 0;
}