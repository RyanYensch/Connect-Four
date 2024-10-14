#include <iostream>
#include <vector>
using namespace std;

const int MaxRows = 6;
const int MaxCol = 7;

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
        for (int j = MaxRows - 1; j >= 0; j--) {
            for (int i = 0; i < MaxCol; i++) {
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

    int countTiles(int row, int col, int drow, int dcol) {
        int same = 0;
        // while (row < )
    }

    bool checkWin(int col) {
        int row = arrayOfVectors[col].size() - 1;




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
            } while ((col < 0 || col >= MaxCol) || arrayOfVectors[col].size() >= MaxCol - 1);

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
