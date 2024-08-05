#include <bits/stdc++.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <queue>
#include <string>

#define MAX_ROWS 10
#define MAX_COLS 10

struct Cell {
    int row;
    int col;
    int position;
};

struct SnakeOrLadder {
    int start;
    int end;
};

struct Pair {
    int val;
    int step;
};

struct Player {
    int position;
    std::string name;
};

void map(const std::vector<std::vector<Cell>>& board, int rows, int cols, int position, int& row, int& col) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j].position == position) {
                row = i;
                col = j;
                return;
            }
        }
    }
    row = -1;
    col = -1;
}

void shortestPath(const std::vector<std::vector<Cell>>& board, int rows, int cols, const std::vector<std::vector<int>>& matrix, int start, int end) {
    std::vector<std::vector<int>> distance(rows, std::vector<int>(cols, INT_MAX));
    std::vector<std::vector<int>> parent(rows, std::vector<int>(cols, -1));
    std::vector<std::vector<int>> diceNumbers(rows, std::vector<int>(cols, -1));
    
    int startRow, startCol;
    map(board, rows, cols, start, startRow, startCol);
    distance[startRow][startCol] = 0;
    
    std::queue<Pair> queue;
    queue.push({start, 0});
    
    while (!queue.empty()) {
        Pair current = queue.front();
        queue.pop();
        int num = current.val;
        int d = current.step;
        
        for (int i = 1; i <= 6; ++i) {
            if (num + i <= rows * cols) {
                int newNum = num + i;
                int newRow, newCol;
                map(board, rows, cols, newNum, newRow, newCol);
                
                if (d + 1 < distance[newRow][newCol]) {
                    parent[newRow][newCol] = num;
                    distance[newRow][newCol] = d + 1;
                    diceNumbers[newRow][newCol] = i;
                    if (matrix[newRow][newCol] == -1) {
                        queue.push({newNum, d + 1});
                    } else {
                        int newVal = matrix[newRow][newCol];
                        int newR, newC;
                        map(board, rows, cols, newVal, newR, newC);
                        parent[newR][newC] = d + 1;
                        queue.push({newVal, d + 1});
                    }
                }
            }
        }
    }
    
    int r, c;
    map(board, rows, cols, rows * cols, r, c);
    std::cout << rows * cols << " ";
    
    std::vector<int> dicePath;
    int pathLength = 0;
    
    while (parent[r][c] != -1) {
        std::cout << "<- ";
        dicePath.push_back(diceNumbers[r][c]);
        std::cout << parent[r][c] << " ";
        std::cout << "<- ";
        map(board, rows, cols, parent[r][c], r, c);
    }
    
    std::cout << "\nMINIMUM NUMBER OF DICE THROWS " << distance[0][0] << "\nDice Numbers Used: ";
    
    for (int i = dicePath.size() - 1; i >= 0; --i) {
        if (dicePath[i] != -1) {
            std::cout << dicePath[i];
        }
    }
    std::cout << "\n";
}

void printBoardWithPlayers(const std::vector<std::vector<Cell>>& board, int rows, int cols, const Player& player1, const Player& player2) {
    for (int i = rows - 1; i >= 0; --i) {
        for (int j = 0; j < cols; ++j) {
            if (player1.position == board[i][j].position) {
                std::cout << "(" << board[i][j].position << ") " << player1.name << "\t";
            } else if (player2.position == board[i][j].position) {
                std::cout << "(" << board[i][j].position << ") " << player2.name << "\t";
            } else {
                std::cout << "(" << board[i][j].position << ")\t";
            }
        }
        std::cout << "\n";
    }
}

void createBoard(std::vector<std::vector<Cell>>& board, int rows, int cols) {
    int num = 1;
    int direction = 1;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = {i, j, 0};
        }
    }
    
    for (int i = rows - 1; i >= 0; --i) {
        if (direction == 1) {
            for (int j = 0; j < cols; ++j) {
                board[i][j].position = num++;
            }
        } else {
            for (int j = cols - 1; j >= 0; --j) {
                board[i][j].position = num++;
            }
        }
        direction = -direction;
    }
}

void createMatrix(const std::vector<std::vector<Cell>>& board, std::vector<std::vector<int>>& matrix, int rows, int cols, const std::vector<SnakeOrLadder>& snakesAndLadders) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = -1;
        }
    }
    for (const auto& sl : snakesAndLadders) {
        int position = sl.start;
        int value = sl.end;
        int row, col;
        map(board, rows, cols, position, row, col);
        matrix[row][col] = value;
    }
}

void printBoard(const std::vector<std::vector<Cell>>& board, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << "(" << board[i][j].row << ", " << board[i][j].col << ") - " << board[i][j].position << "\t";
        }
        std::cout << "\n";
    }
}

void createRandomSnakesAndLadders(std::vector<SnakeOrLadder>& snakesAndLadders, int numSnakesAndLadders, int totalCells) {
    srand(static_cast<unsigned>(time(nullptr)));
    
    for (int i = 0; i < numSnakesAndLadders; ++i) {
        int start = rand() % (totalCells - 1) + 1;
        int end = rand() % (totalCells - start) + (start + 1);
        
        snakesAndLadders[i] = {start, end};
    }
}

void playGame(const std::vector<std::vector<Cell>>& board, int rows, int cols, const std::vector<std::vector<int>>& matrix) {
    Player player1 = {1, "P1"};
    Player player2 = {1, "P2"};
    int goal = rows * cols;
    bool player1Turn = true;
    
    while (player1.position != goal && player2.position != goal) {
        int diceRoll;
        if (player1Turn) {
            std::cout << "Press enter to roll the dice...";
            std::cin.get();
            diceRoll = rand() % 6 + 1;
            std::cout << player1.name << " rolled a " << diceRoll << "\n";
            player1.position += diceRoll;
            if (player1.position > goal) {
                player1.position = goal;
            }
            int row, col;
            map(board, rows, cols, player1.position, row, col);
            if (matrix[row][col] != -1) {
                player1.position = matrix[row][col];
            }
            std::cout << player1.name << " moved to " << player1.position << "\n";
        } else {
            diceRoll = rand() % 6 + 1;
            std::cout << player2.name << " rolled a " << diceRoll << "\n";
            player2.position += diceRoll;
            if (player2.position > goal) {
                player2.position = goal;
            }
            int row, col;
            map(board, rows, cols, player2.position, row, col);
            if (matrix[row][col] != -1) {
                player2.position = matrix[row][col];
            }
            std::cout << player2.name << " moved to " << player2.position << "\n";
        }
        player1Turn = !player1Turn;
        printBoardWithPlayers(board, rows, cols, player1, player2);
    }
    if (player1.position == goal) {
        std::cout << "Player 1 is the Winner\n";
    } else {
        std::cout << "Player 2 is the Winner\n";
    }
}

int main() {
    int rows = 10;
    int cols = 10;
    std::vector<std::vector<Cell>> board(rows, std::vector<Cell>(cols));
    createBoard(board, rows, cols);
    
    int numSnakesAndLadders = 10;
    std::vector<SnakeOrLadder> snakesAndLadders(numSnakesAndLadders);
    createRandomSnakesAndLadders(snakesAndLadders, numSnakesAndLadders, rows * cols);
    
    printBoard(board, rows, cols);
    
    std::cout << "Snake and Ladder Positions:\n";
    for (const auto& sl : snakesAndLadders) {
        std::cout << "(" << sl.start << "," << sl.end << ") ";
    }
    std::cout << "\n";
    
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, -1));
    createMatrix(board, matrix, rows, cols, snakesAndLadders);
    
    std::cout << "Matrix with snake and ladder values:\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << "\t";
        }
        std::cout << "\n";
    }
    
    while (true) {
        std::cout << "\nChoose an option:\n";
        std::cout << "1. Find the shortest path\n";
        std::cout << "2. Play the game\n";
        std::cout << "3. Generate the random position of snake and ladder\n";
        std::cout << "4. Quit\n";
        
        int choice;
        std::cout << "Your choice is: ";
        std::cin >> choice;
        std::cin.ignore();  // To ignore the newline character after entering the choice
        
        switch (choice) {
            case 1:
                std::cout << "\nFinding the shortest path:\n";
                shortestPath(board, rows, cols, matrix, 1, rows * cols);
                break;
            case 2:
                std::cout << "\nPlaying the game:\n";
                playGame(board, rows, cols, matrix);
                break;
            case 3:
                createRandomSnakesAndLadders(snakesAndLadders, numSnakesAndLadders, rows * cols);
                createMatrix(board, matrix, rows, cols, snakesAndLadders);
                std::cout << "Matrix with snake and ladder values:\n";
                for (const auto& row : matrix) {
                    for (int val : row) {
                        std::cout << val << "\t";
                    }
                    std::cout << "\n";
                }
                break;
            case 4:
                return 0;
            default:
                std::cout << "Invalid choice. Please select a valid option.\n";
                break;
        }
    }
}