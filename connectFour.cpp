/*
#!/bin/bash
g++ "$0" -o "${0%.cpp}" && exec "${0%.cpp}" "$@"
*/


#include <iostream>
#include <vector>
using namespace std;

const int MaxRows = 6;
const int MaxCol = 7;
const char Player1char = 'X';
const char Player2char = 'O';

struct GameState {
    int playerCount;
    int lastCol;
    int playerTurn;
    bool isGameOver;
};

class ConnectFour {
    private:
    vector<vector<char>> arrayOfVectors;  // Declare it without initialization
    char playerChar[2] = {Player1char, Player2char};

    public:
    // Constructor with member initializer list
    ConnectFour() : arrayOfVectors(7) {}

    private:
    // Clears the screen
    void clearScreen() {
        cout << "\033[2J\033[H";
    }

    // Prints the board
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

    // adds players move to the game
    void playerMove(int playerID, int col) {
        arrayOfVectors[col].push_back(playerChar[playerID]);
    }

    // counts the tiles in a direction
    int countTiles(int row, int col, int drow, int dcol) {
        int same = 0;
        char compare = arrayOfVectors[col].at(row);

        for (int i = 0; i < 2; i++) {
            int tempcol = col;
            int temprow = row;
            while (tempcol >= 0 && temprow >= 0 && tempcol < MaxCol && temprow < arrayOfVectors[tempcol].size() && arrayOfVectors[tempcol].at(temprow) == compare) {
                same++;
                temprow += drow;
                tempcol += dcol;
            }
            drow *= -1;
            dcol *= -1;
        }
        
        // counts the intial tile twice
        return same - 1;
    }

    // checks if there is a 4+ in a row
    bool checkWin(int col) {
        int row = arrayOfVectors[col].size() - 1;
        string player = (arrayOfVectors[col].at(row) == playerChar[0]) ? "Player 1": "Player 2";
        clearScreen();
        printBoard();

        if (countTiles(row, col, 0, 1) >= 4) {
            cout << player << " won horizontally!\n";
            return true;
        } else if (countTiles(row, col, 1, 0) >= 4) {
            cout << player << " won vertically!\n";
            return true;
        } else if (countTiles(row, col, 1, 1) >= 4 || countTiles(row, col, 1, -1) >= 4) {
            cout << player << " won diagonally!\n";
            return true;
        } 


        return false;
    }


    public:
    // Lets the class start the game and loop
    void playGame() {
        int col;
        int turn = 0;
        clearScreen();
        printBoard();
        do {
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
