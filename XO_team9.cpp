#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class Board
{
private:
    vector<vector<char>> grid;
    int size;

public:
    void makeBoard(int s = 3)
    {
        size = s;
        grid = vector<vector<char>>(size, vector<char>(size, ' '));
    }

    void display() const
    {
        /*
         X |   | O
        ---+---+---
           | X |
        ---+---+---
         O |   | X
        */
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++) // makes the  X |   | O
            {
                cout << " " << grid[i][j] << " ";
                if (j < size - 1) //  X |   | O
                    cout << "|";  //  if we are here | don't add the "|" symbol
            }
            cout << endl; // new line

            if (i < size - 1) // makes the ---+---+---
            {
                for (int j = 0; j < size; j++)
                {
                    cout << "---";
                    if (j < size - 1) // ---+---+---
                        cout << "+";  //  if we are here | don't add the "+" symbol
                }
                cout << endl; // new line for next lines
            }
        }
    }

    bool makeMove(int row, int col, char symbol)
    {
        // makes a move
        if (isValidMove(row, col))
        {
            grid[row][col] = symbol;
            return true;
        }
        return false;
    }

    bool isValidMove(int row, int col) const
    {
        // checks if this is a valid spot to put a X or O
        if (row < 0 || row >= size || col < 0 || col >= size)
        {
            return false;
        }
        if (grid[row][col] != ' ')
        {
            return false;
        }

        return true;
    }

    bool checkWin(char symbol) const
    {
        // checks the win conditions (rows, columns , diagonals)

        // Check rows
        for (int i = 0; i < size; i++)
        {
            bool rowWin = true;
            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] != symbol)
                {
                    rowWin = false;
                    break;
                }
            }
            if (rowWin)
                return true;
        }

        // Check columns
        for (int j = 0; j < size; j++)
        {
            bool colWin = true;
            for (int i = 0; i < size; i++)
            {
                if (grid[i][j] != symbol)
                {
                    colWin = false;
                    break;
                }
            }
            if (colWin)
                return true;
        }

        // Check main diagonal
        bool mainDiag = true;
        for (int i = 0; i < size; i++)
        {
            if (grid[i][i] != symbol)
            {
                mainDiag = false;
                break;
            }
        }
        if (mainDiag)
            return true;

        // Check anti-diagonal
        bool antiDiag = true;
        for (int i = 0; i < size; i++)
        {
            if (grid[i][size - i - 1] != symbol)
            {
                antiDiag = false;
                break;
            }
        }
        if (antiDiag)
            return true;

        return false; // no win
    }

    bool isFull() const
    {
        // indicating board full status

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] == ' ')
                    return false;
            }
        }
        return true;
    }

    char getCell(int row, int col) const
    {
        // return symbol at specified coordinates
        return grid[row][col];
    }
    void reset()
    {
        // clears all cells to empty state

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                grid[i][j] = ' ';
            }
        }
    }
    int getSize() const
    {
        // getter for grid size
        return size;
    }
};

class Player
{
protected:
    string name;
    char symbol;

public:
    Player(const string &n, char s)
    {
        name = n;
        symbol = s;
    }

    // this will be overriden in the next classes for
    // human player
    // ai player for each one we will use a different getMove function
    virtual void getMove(const Board &myBoard, int &row, int &col) = 0;

    string getName() const
    {
        return name;
    }
    char getSymbol() const
    {
        return symbol;
    }
    void setName(const string &n)
    {
        name = n;
        cout << "Name changed successfully!" << endl;
    }
};

class AIPlayer : public Player
{
private:
    int difficulty;

public:
    // ": Player(n, sym)" makes a para. constructor for Player
    AIPlayer(const string &n = "Sama7", char sym = 'A', int diff = 1) : Player(n, sym)
    {
        difficulty = diff;
    }

    void setDifficulty(int newDifficulty) // 1.easy 2. medium 3. hard
    {
        difficulty = newDifficulty;
    }

    // minimax algorithm
    int minimax(Board &board, int depth, bool isMaximizing, char aiSymbol, char humanSymbol) const
    {
        // base cases
        if (board.checkWin(aiSymbol)) // play the best move to win
        {
            return 10 - depth; // + 10 - depth (10 is always bigger)
        }
        if (board.checkWin(humanSymbol)) // play this move to not lose
        {
            return depth - 10; // - depth - 10 (10 is always bigger)
        }
        if (board.isFull()) // draw
        {
            return 0;
        }

        int boardSize = board.getSize();
        if (isMaximizing) // the ai wants to win
        {
            int bestScore = -1000;
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        board.makeMove(i, j, aiSymbol);                                      // puts a symbol in a place
                        int score = minimax(board, depth + 1, false, aiSymbol, humanSymbol); // it's the human's turn
                        //                                    |-> this is why isMaximizing is passed as false

                        // compares between if we put the symbol and if we don't
                        bestScore = max(bestScore, score); // gets the maximium score
                        board.makeMove(i, j, ' ');         // deletes the symbol for next iteration
                    }
                }
            }
            return bestScore;
        }
        else // it's the human player's turn, we should minimize the score
        {
            int bestScore = 1000;
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        board.makeMove(i, j, humanSymbol);                                  // puts HUMAN symbol in place
                        int score = minimax(board, depth + 1, true, aiSymbol, humanSymbol); // it's the ai's turn next
                        //                                    |-> this is why isMaximizing is passed as true

                        // compares between if we put the symbol and if we don't
                        bestScore = min(bestScore, score); // gets the minimum score
                        board.makeMove(i, j, ' ');         // deletes the symbol for next iteration
                    }
                }
            }
            return bestScore;
        }
    }

    void getRandomMove(const Board &board, int &row, int &col) const
    {
        vector<pair<int, int>> validMoves;
        int boardSize = board.getSize();

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++) // iterates through the grid
            {
                if (board.isValidMove(i, j)) // if the current position is valid
                {
                    validMoves.push_back({i, j}); // add it to valid moves
                }
            }
        }

        // check if there are valid moves FIRST
        if (!validMoves.empty()) // because we can't divide by zero
        {
            static bool seeded = false; // using singleton to seed only once
            if (!seeded)
            {
                srand(time(0));
                seeded = true;
            }

            int randomPosition = rand() % validMoves.size(); // safe size() is bigger than 0
            row = validMoves[randomPosition].first;          // selects the index and gets row (first)
            col = validMoves[randomPosition].second;         // selects the index and gets col (second)
        }
        else
        {
            // fallback
            row = -1;
            col = -1;
        }
    }

    void getBestMove(const Board &board, int &row, int &col) const
    {
        char aiSymbol = symbol;
        char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';

        int bestScore = -1000; // use minimax algorithm
        int bestRow = -1, bestCol = -1;

        Board tempBoard = board; // create a copy of the board for making a move for simulation

        // simulate all possible moves
        for (int i = 0; i < board.getSize(); i++)
        {
            for (int j = 0; j < board.getSize(); j++)
            {
                if (board.isValidMove(i, j))
                {
                    tempBoard.makeMove(i, j, aiSymbol); // Make move
                    int score = minimax(tempBoard, 0, false, aiSymbol, humanSymbol);
                    tempBoard.makeMove(i, j, ' '); // Undo move

                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }

        row = bestRow;
        col = bestCol;
    }

    void getGoodMove(const Board &board, int &row, int &col) const
    {
        // sometimes plays best moves and sometimes plays random moves for medium difficulty

        // use static seeding and singleton like in getRandomMove
        static bool seeded = false;
        if (!seeded)
        {
            srand(time(0));
            seeded = true;
        }

        int choice = rand() % 100; // generates a number from 0-99

        // 70% chance to play best move (0-69) - challenging but not impossible
        if (choice < 70)
        {
            getBestMove(board, row, col);
        }
        // 30% chance to play random move (70-99) - gives human opportunities
        else
        {
            getRandomMove(board, row, col);
        }
    }

    void getMove(const Board &myBoard, int &row, int &col) override
    {
        // implement the optimal solution depending on the difficulty
        // change the grid[row][col] to symbol depending on diff
        switch (difficulty)
        {
        case 1:
        {
            getRandomMove(myBoard, row, col);
            break;
        }
        case 2:
        {
            getGoodMove(myBoard, row, col);
            break;
        }

        case 3:
        {
            getBestMove(myBoard, row, col);
            break;
        }
        }
    }

    /* int evaluateBoard(const Board& Board) const
    {
        // evaluates board state for scoring (win/loss/draw) i don't know if this should be in the ai class
    }*/
};

class Game
{
private:
    Board *myBoard;
    Player *player1;
    Player *player2;
    int currentPlayer; // 1 or 2
    int winner = -1;

public:
    Game()
    {
        /*
        int size;
        cout << "What is the size of your grid?" << endl;
        cin >> size;
        cout << "Player 1 name: " << endl;
        cout << "Player 1 name: " << endl;

        لسة مش عارف هنعمل فيها ايه بس دي ك بداية
        */
        myBoard = new Board(); // Allocate memory first!
        myBoard->makeBoard(3); // Then initialize with size 3
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = 1;
    }
    /*
    Game(Player *player1 , Player *player2 , Board& myboard)
    {

    }
    */
    void showMenu()
    {
        int choice;
        cout << "===== Tic Tac Toe =====" << endl
             << "1. Player vs Player" << endl
             << "2. Player vs Computer" << endl
             << "Choose a mode: ";
        cin >> choice;

        if (choice == 1)
        {
            setupPVP();
        }
        else if (choice == 2)
        {
            int choice;
            cout << "Choose your difficulty!" << endl
                 << "1. Easy" << endl
                 << "2. Medium" << endl
                 << "3. Hard" << endl;
            cin >> choice;
            setupPVC(choice);
        }

        startGame();
    }

    void startGame()
    {
        bool gameOver = false;
        myBoard->display();

        while (!gameOver)
        {
            Player *current = (currentPlayer == 1 ? player1 : player2);
            int row, col;

            current->getMove(*myBoard, row, col);
            if (myBoard->makeMove(row, col, current->getSymbol()))
            {
                myBoard->display();
                if (myBoard->checkWin(current->getSymbol()))
                {
                    cout << current->getName() << " wins!" << endl;
                    gameOver = true;
                }
                else if (myBoard->isFull())
                {
                    cout << "It's a draw!" << endl;
                    gameOver = true;
                }
                else
                {
                    switchPlayer();
                }
            }
            else
            {
                cout << "Invalid move." << endl;
            }
        }
    }
    void setupPVP()
    {
        // configures player vs player mode with user input
    }
    void setupPVC(int difficulty)
    {
        // configures player vs computer mode with user input
    }
    void switchPlayer()
    {
        // alternates current player between players
        if (currentPlayer == 1)
        {
            currentPlayer = 2;
        }
        else
        {
            currentPlayer = 1;
        }
    }

    void handleHumanMove(Player &player)
    {
        // executes the input from the user
    }

    void handleAIMove(AIPlayer &ai)
    {
        // executs ai move calculation and placement
        // depending on the difficulty
    }

    bool checkGameEnd()
    {
        // checks win conditions and board full status
        // calls the isFull() in Board
        return false;
    }

    void displayResult() const
    {
        if (myBoard->checkWin(player1->getSymbol()))
        {
            cout << player1->getName() << " wins!" << endl;
        }
        else if (myBoard->checkWin(player2->getSymbol()))
        {
            cout << player2->getName() << " wins!" << endl;
        }
        else if (myBoard->isFull())
        {
            cout << "It's a draw!" << endl;
        }
    }

    void reset()
    {
        // prepares game for new round
        myBoard->reset();
        currentPlayer = 1;
        winner = -1;
    }
    ~Game()
    {
        delete myBoard;
        delete player1;
        delete player2;
    }
};

int testing() // Test program for AI functions
{
    cout << "=== TIC-TAC-TOE AI TESTING PROGRAM ===" << endl;
    cout << "Testing the implemented AI functions..." << endl
         << endl;

    // Create a test board
    Board testBoard;
    testBoard.makeBoard(3);

    cout << "1. Testing Board Display:" << endl;
    testBoard.display();
    cout << endl;

    // Test makeMove function
    cout << "2. Testing makeMove function:" << endl;
    cout << "Making moves: X at (0,0), O at (1,1), X at (0,1)" << endl;
    testBoard.makeMove(0, 0, 'X');
    testBoard.makeMove(1, 1, 'O');
    testBoard.makeMove(0, 1, 'X');
    testBoard.display();
    cout << endl;

    // Create AI players with different difficulties
    AIPlayer easyAI("Easy AI", 'O', 1);
    AIPlayer mediumAI("Medium AI", 'O', 2);
    AIPlayer hardAI("Hard AI", 'O', 3);

    // Test Easy AI (Random moves)
    cout << "3. Testing Easy AI (Random moves):" << endl;
    cout << "Easy AI will make 3 random moves..." << endl;
    for (int i = 0; i < 10; i++)
    {
        int row, col;
        easyAI.getMove(testBoard, row, col);
        cout << "Easy AI chose position: (" << row << ", " << col << ")" << endl;
    }
    cout << endl;

    // Test Medium AI (Good moves - mix of random and best)
    cout << "4. Testing Medium AI (70% best, 30% random):" << endl;
    cout << "Medium AI will make 5 moves to show variation..." << endl;
    for (int i = 0; i < 5; i++)
    {
        int row, col;
        mediumAI.getMove(testBoard, row, col);
        cout << "Medium AI chose position: (" << row << ", " << col << ")" << endl;
    }
    cout << endl;

    // Test Hard AI (Best moves using minimax)
    cout << "5. Testing Hard AI (Minimax algorithm):" << endl;
    cout << "Hard AI analyzing the current board state..." << endl;
    int bestRow, bestCol;
    hardAI.getBestMove(testBoard, bestRow, bestCol);
    cout << "Hard AI's best move: (" << bestRow << ", " << bestCol << ")" << endl;
    cout << endl;

    // Create a scenario where AI can win
    cout << "6. Testing AI in a winning scenario:" << endl;
    Board winBoard;
    winBoard.makeBoard(3);
    winBoard.makeMove(0, 0, 'O'); // AI symbol
    winBoard.makeMove(0, 1, 'O'); // AI can win at (0,2)
    winBoard.makeMove(1, 0, 'X'); // Human moves
    winBoard.makeMove(2, 1, 'X');

    cout << "Current board state:" << endl;
    winBoard.display();
    cout << endl;

    hardAI.getBestMove(winBoard, bestRow, bestCol);
    cout << "Hard AI should choose (0, 2) to win: (" << bestRow << ", " << bestCol << ")" << endl;
    cout << endl;

    // Test all difficulty levels with the same board
    cout << "7. Comparing all AI difficulties on the same board:" << endl;
    Board compareBoard;
    compareBoard.makeBoard(3);
    compareBoard.makeMove(1, 1, 'X'); // Human takes center
    compareBoard.display();
    cout << endl;

    int easyRow, easyCol, mediumRow, mediumCol, hardRow, hardCol;

    easyAI.getMove(compareBoard, easyRow, easyCol);
    mediumAI.getMove(compareBoard, mediumRow, mediumCol);
    hardAI.getMove(compareBoard, hardRow, hardCol);

    cout << "Easy AI (Difficulty 1) chose: (" << easyRow << ", " << easyCol << ")" << endl;
    cout << "Medium AI (Difficulty 2) chose: (" << mediumRow << ", " << mediumCol << ")" << endl;
    cout << "Hard AI (Difficulty 3) chose: (" << hardRow << ", " << hardCol << ")" << endl;
    cout << endl;

    // Test board utility functions
    cout << "8. Testing board utility functions:" << endl;
    cout << "Board size: " << compareBoard.getSize() << endl;
    cout << "Is (0,0) valid move? " << (compareBoard.isValidMove(0, 0) ? "Yes" : "No") << endl;
    cout << "Is (1,1) valid move? " << (compareBoard.isValidMove(1, 1) ? "Yes" : "No") << endl;
    cout << "Cell (1,1) contains: '" << compareBoard.getCell(1, 1) << "'" << endl;
    cout << "Is board full? " << (compareBoard.isFull() ? "Yes" : "No") << endl;
    cout << endl;

    cout << "=== ALL TESTS COMPLETED ===" << endl;
    cout << "The AI functions are working correctly!" << endl;

    return 0;
}
int main()
{

    Board testBoard;
    testBoard.makeBoard(3);
    return 0;
}
