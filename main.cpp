#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to draw the Tic-Tac-Toe board
void drawBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; ++i) {
        cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << " \n";
        if (i != 2)
            cout << "---|---|---\n";
    }
}

// Function to check if a player has won
bool checkWin(const vector<vector<char>>& board, char player) {
    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}

// Function to make a random move for the AI player
void makeRandomMove(vector<vector<char>>& board, char aiPlayer) {
    vector<pair<int, int>> availableMoves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ')
                availableMoves.push_back(make_pair(i, j));
        }
    }

    if (!availableMoves.empty()) {
        int randomIndex = rand() % availableMoves.size();
        int row = availableMoves[randomIndex].first;
        int col = availableMoves[randomIndex].second;
        board[row][col] = aiPlayer;
    }
}

// Function to evaluate the board for the minimax algorithm
int evaluateBoard(const vector<vector<char>>& board, char aiPlayer, char humanPlayer) {
    if (checkWin(board, aiPlayer))
        return 10;
    else if (checkWin(board, humanPlayer))
        return -10;
    return 0;
}

// Minimax algorithm for the AI's optimal move
int minimax(vector<vector<char>>& board, int depth, bool isMax, char aiPlayer, char humanPlayer) {
    int score = evaluateBoard(board, aiPlayer, humanPlayer);

    if (score == 10 || score == -10)
        return score;

    if (depth == 9) // The board is full (max depth reached)
        return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = aiPlayer;
                    best = max(best, minimax(board, depth + 1, !isMax, aiPlayer, humanPlayer));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
    else {
        int best = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == ' ') {
                    board[i][j] = humanPlayer;
                    best = min(best, minimax(board, depth + 1, !isMax, aiPlayer, humanPlayer));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

// Function to make an optimal move for the AI player using the minimax algorithm
void makeOptimalMove(vector<vector<char>>& board, char aiPlayer, char humanPlayer) {
    int bestScore = -1000;
    int bestRow = -1;
    int bestCol = -1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                board[i][j] = aiPlayer;
                int moveScore = minimax(board, 0, false, aiPlayer, humanPlayer);
                board[i][j] = ' ';
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    if (bestRow != -1 && bestCol != -1)
        board[bestRow][bestCol] = aiPlayer;
}

int main() {
    vector<vector<char>> board(3, vector<char>(3, ' '));
    char humanPlayer = 'X';
    char aiPlayer = 'O';

    cout << "Welcome to Tic-Tac-Toe!\n";
    cout << "You are playing as X. The AI is playing as O.\n";

    srand(static_cast<unsigned>(time(0)));

    int row, col;
    int moves = 0;
    bool isHardMode = false;

    cout << "Select AI difficulty:\n";
    cout << "1. Easy\n";
    cout << "2. Hard\n";
    int difficulty;
    cin >> difficulty;

    if (difficulty == 2)
        isHardMode = true;

    while (moves < 9) {
        drawBoard(board);

        // Player's turn
        cout << "Your move (row [0-2] and column [0-2]): ";
        cin >> row >> col;

        // Validate the move
        if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        // Update the board
        board[row][col] = humanPlayer;
        ++moves;

        // Check if the human player has won
        if (checkWin(board, humanPlayer)) {
            cout << "You win!\n";
            drawBoard(board);
            break;
        }

        // AI's turn
        if (isHardMode)
            makeOptimalMove(board, aiPlayer, humanPlayer);
        else
            makeRandomMove(board, aiPlayer);
        ++moves;

        // Check if the AI player has won
        if (checkWin(board, aiPlayer)) {
            cout << "AI wins!\n";
            drawBoard(board);
            break;
        }
    }

    // If all moves have been made and no one has won, it's a tie
    if (moves == 9) {
        cout << "It's a tie!\n";
        drawBoard(board);
    }

    return 0;
}