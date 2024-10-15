/*
#!/bin/bash
g++ "$0" -o "${0%.cpp}" && exec "${0%.cpp}" "$@"
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <thread>
#include <chrono>
using namespace std;

const int MaxRows = 6;
const int MaxCol = 7;
const char Player1char = 'X';
const char Player2char = 'O';
const string filename = "DataStore.txt";

struct GameState {
    int playerCount;
    int lastCol;
    int playerTurn;
    bool isGameOver;
};

namespace utils {
    void delay(double seconds) {
        this_thread::sleep_for(chrono::duration<double>(seconds));
    }
}

class ConnectFour {
    private:
    vector<vector<char>> arrayOfVectors;  // Declare it without initialization
    char playerChar[2] = {Player1char, Player2char};
    int playerId;

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

    void saveGamesToFile(const map<int, GameState>& gamesMap) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: couldn't open file\n";
            return;
        }

        for (const auto& pair: gamesMap) {
            file << pair.first << " " << pair.second.playerCount << " " 
             << pair.second.lastCol << " " << pair.second.playerTurn << " "
             << pair.second.isGameOver << endl;
        }

        file.close();
    }

    map<int, GameState> loadGamesFromFile() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: couldn't open file\n";
            return {};
        }

        map<int, GameState> gamesMap;
        int gameId, playerCount, lastCol, playerTurn;
        bool isGameOver;

        while (file >> gameId >> playerCount >> lastCol >> playerTurn >> isGameOver) {
            GameState state = {playerCount, lastCol, playerTurn, isGameOver};
            gamesMap[gameId] = state;
        }
        
        file.close();
        return gamesMap;
    }

    void createGame(int gameId) {
        map<int, GameState> gamesMap = loadGamesFromFile();
        playerId = 0;
        gamesMap[gameId] = {1, -1, 0, false};
        saveGamesToFile(gamesMap);
        playMultiGame(gameId);
    }

    void playMultiGame(int gameId) {
        map<int, GameState> gamesMap = loadGamesFromFile();
        int col;
        bool isWinner = false;
        while (gamesMap[gameId].isGameOver == false) {
            while (gamesMap[gameId].playerTurn != playerId) {
                utils::delay(0.5);
                gamesMap = loadGamesFromFile();
            }
            if (gamesMap[gameId].isGameOver == true) break;

            if (gamesMap[gameId].lastCol != -1) {
                playerMove((playerId + 1) % 2, gamesMap[gameId].lastCol);
            }
            clearScreen();
            printBoard();

            do {
                cout << "Which Column (0-6): ";
                cin >> col;
            } while ((col < 0 || col >= MaxCol) || arrayOfVectors[col].size() >= MaxCol - 1);

            playerMove(playerId, col);
            gamesMap[gameId].lastCol = col;
            gamesMap[gameId].playerTurn = (playerId + 1) % 2;
            gamesMap[gameId].isGameOver = checkWin(col);
            isWinner = gamesMap[gameId].isGameOver;

            saveGamesToFile(gamesMap);
            if (gamesMap[gameId].isGameOver == true) break;

            if (!gamesMap[gameId].isGameOver) {
                cout << "Waiting for opponents move\n";
            }
        }

        if (!isWinner) {
            clearScreen();
            printBoard();
            cout << "Your Opponent Won!\n";
        }
        cout << "Game Over\n";
        
    }

    public:
    void joinGame(int gameId) {
        map<int, GameState> gamesMap = loadGamesFromFile();
        if (gamesMap.count(gameId) > 0 && gamesMap[gameId].playerCount < 2) {
            gamesMap[gameId].playerCount++;
            saveGamesToFile(gamesMap);
            playerId = 1;
            clearScreen();
            printBoard();
            cout << "Waiting for opponents move\n";
            playMultiGame(gameId);
        } else if (gamesMap[gameId].playerCount >= 2 && gamesMap[gameId].isGameOver == false) {
            cout << "Game is full.\nEnter new Game Id: ";
            cin >> gameId;
            joinGame(gameId);
        } else {
            createGame(gameId);  
        }
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
    int gameId;
    cout << "Enter Game Id (-1 for same terminal): ";
    cin >> gameId;
    ConnectFour game;
    if (gameId != -1) {
        game.joinGame(gameId);
    } else {
        game.playGame();
    }
    
   
    return 0;
}
