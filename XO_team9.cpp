#include <iostream>
#include <vector> // line 481
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
    int winCondition;

public:
    // Default constructor
    Board() : size(3), winCondition(3)
    {
        grid = vector<vector<char>>(size, vector<char>(size, ' '));
    }

    void makeBoard(int s = 3)
    {
        size = s;
        if (s == 3)
            winCondition = 3; // 3x3: need win is 3 in a row
        else if (s == 4)
            winCondition = 4; // 4x4: need win is 4 in a row
        else if (s == 5)
            winCondition = 4; // 5x5: need win is 4 in a row

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
        row--;
        col--;
        if (isValidMove(row, col))
        {
            grid[row][col] = symbol;
            return true;
        }
        return false;
    }

    // copy operator
    Board &operator=(const Board &other)
    {
        if (this != &other) // check if not needed to do copy (if equal)
        {
            this->makeBoard(other.getSize()); // initialize with the same size

            // copy the board
            for (int i = 0; i < other.getSize(); i++)
            {
                for (int j = 0; j < other.getSize(); j++)
                {
                    grid[i][j] = other.grid[i][j];
                }
            }
        }
        return *this;
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
        // Check rows
        for (int i = 0; i < size; i++)
        {
            int count = 0;
            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] == symbol)
                {
                    count++;
                    if (count >= winCondition)
                        return true;
                }
                else
                {
                    count = 0; // reset count if symbol is different to begin the next iteration
                }
            }
        }

        // Check columns
        for (int j = 0; j < size; j++)
        {
            int count = 0;
            for (int i = 0; i < size; i++)
            {
                if (grid[i][j] == symbol)
                {
                    count++;
                    if (count >= winCondition)
                        return true;
                }
                else
                {
                    count = 0; // reset count if symbol is different to begin the next iteration
                }
            }
        }

        // check main diagonals (top-left to bottom-right)
        for (int startRow = 0; startRow <= size - winCondition; startRow++)
        {
            for (int startCol = 0; startCol <= size - winCondition; startCol++)
            {
                int count = 0;
                for (int i = 0; i < winCondition; i++)
                {
                    if (grid[startRow + i][startCol + i] == symbol)
                        count++;
                    else
                        break;
                }
                if (count == winCondition)
                    return true;
            }
        }

        // check anti-diagonals (top-right to bottom-left)
        for (int startRow = 0; startRow <= size - winCondition; startRow++)
        {
            for (int startCol = winCondition - 1; startCol < size; startCol++)
            {
                int count = 0;
                for (int i = 0; i < winCondition; i++)
                {
                    if (grid[startRow + i][startCol - i] == symbol)
                        count++;
                    else
                        break;
                }
                if (count == winCondition)
                    return true;
            }
        }

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

    int getWinCondition() const
    {
        // getter for win condition
        return winCondition;
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
    virtual void setDifficulty(int newDifficulty) = 0;

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

class HumanPlayer : public Player
{
public:
    HumanPlayer(const string &n, char s) : Player(n, s) {}

    void getMove(const Board &myBoard, int &row, int &col) override
    {
        bool selectingIndex = true;
        while (selectingIndex)
        {
            cout << "Enter your row (1-" << myBoard.getSize() << "): ";
            cin >> row;
            cout << "Enter your column (1-" << myBoard.getSize() << "): ";
            cin >> col;
            cout << endl;
            if (row < 1 || row > myBoard.getSize() || col < 1 || col > myBoard.getSize())
            {
                cout << "Invalid row or column!" << endl;
            }
            else
                selectingIndex = false;
        }
    }
    void setDifficulty(int newDifficulty) override {}
};

class AIPlayer : public Player
{
private:
    int difficulty;

public:
    // ": Player(n, sym)" makes a para. constructor for Player
    AIPlayer(const string &n, char sym, int diff) : Player(n, sym)
    {
        difficulty = diff;
    }

    void setDifficulty(int newDifficulty) override // 1.easy 2. medium 3. hard
    {
        difficulty = newDifficulty;
    }

    // minimax algorithm for finding the best move
    int minimax(Board &board, int depth, bool isMaximizing, char aiSymbol, char humanSymbol) const
    {
        // check if game is over
        if (board.checkWin(aiSymbol))
        {
            return 1000 - depth; // win faster is better
        }
        if (board.checkWin(humanSymbol))
        {
            return -1000 + depth; // lose slower is better
        }
        if (board.isFull())
        {
            return 0; // draw
        }

        // made maximum search depths for each grid size
        int maxDepth;
        int boardSize = board.getSize();
        if (boardSize == 3)
            maxDepth = 9; // best depth for 3x3
        else if (boardSize == 4)
            maxDepth = 7; // balanced search for 4x4
        else if (boardSize == 5)
            maxDepth = 5; // good search for 5x5

        if (depth >= maxDepth)
        {
            return evaluatePosition(board, aiSymbol, humanSymbol);
        }

        if (isMaximizing) // ai wants the highest score
        {
            int bestScore = -1000;
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        board.makeMove(i, j, aiSymbol); // Make AI move
                        int score = minimax(board, depth + 1, false, aiSymbol, humanSymbol);
                        board.makeMove(i, j, ' '); // Undo move
                        bestScore = max(bestScore, score);
                    }
                }
            }
            return bestScore;
        }
        else // human wants the lowest score
        {
            int bestScore = 1000;
            for (int i = 0; i < boardSize; i++)
            {
                for (int j = 0; j < boardSize; j++)
                {
                    if (board.isValidMove(i, j))
                    {
                        board.makeMove(i, j, humanSymbol); // Make human move
                        int score = minimax(board, depth + 1, true, aiSymbol, humanSymbol);
                        board.makeMove(i, j, ' '); // Undo move
                        bestScore = min(bestScore, score);
                    }
                }
            }
            return bestScore;
        }
    }

    // figure out how good this board position is
    int evaluatePosition(const Board &board, char aiSymbol, char humanSymbol) const
    {
        int score = 0;
        int boardSize = board.getSize();
        int winCondition = board.getWinCondition();

        // check all the ways to win
        // check rows
        for (int i = 0; i < boardSize; i++)
        {
            score += evaluateLine(board, i, 0, 0, 1, aiSymbol, humanSymbol, winCondition);
        }

        // check columns
        for (int j = 0; j < boardSize; j++)
        {
            score += evaluateLine(board, 0, j, 1, 0, aiSymbol, humanSymbol, winCondition);
        }

        // check diagonals
        for (int i = 0; i <= boardSize - winCondition; i++)
        {
            for (int j = 0; j <= boardSize - winCondition; j++)
            {
                score += evaluateLine(board, i, j, 1, 1, aiSymbol, humanSymbol, winCondition);
                if (j + winCondition - 1 < boardSize)
                    score += evaluateLine(board, i, j + winCondition - 1, 1, -1, aiSymbol, humanSymbol, winCondition);
            }
        }

        // some bonuses based on grid size for optimizing mini max
        if (boardSize == 4)
        {
            // 4x4 grid: center control is very important
            int center = boardSize / 2;
            for (int i = center - 1; i <= center; i++)
            {
                for (int j = center - 1; j <= center; j++)
                {
                    if (board.getCell(i, j) == aiSymbol)
                        score += 10; // Higher bonus for 4x4 center
                    else if (board.getCell(i, j) == humanSymbol)
                        score -= 10;
                }
            }

            // corner bonuses for 4x4
            int corners[4][2] = {{0, 0}, {0, 3}, {3, 0}, {3, 3}};
            for (int k = 0; k < 4; k++)
            {
                if (board.getCell(corners[k][0], corners[k][1]) == aiSymbol)
                    score += 6;
                else if (board.getCell(corners[k][0], corners[k][1]) == humanSymbol)
                    score -= 6;
            }
        }
        else if (boardSize == 5)
        {
            // 5x5 grid: center and edges are better
            int center = 2; // center of 5x5 is (2,2)
            if (board.getCell(center, center) == aiSymbol)
                score += 15; // bonus for exact center
            else if (board.getCell(center, center) == humanSymbol)
                score -= 15;

            // next to center positions
            int centerAdjacent[4][2] = {{1, 2}, {2, 1}, {2, 3}, {3, 2}};
            for (int k = 0; k < 4; k++)
            {
                if (board.getCell(centerAdjacent[k][0], centerAdjacent[k][1]) == aiSymbol)
                    score += 8;
                else if (board.getCell(centerAdjacent[k][0], centerAdjacent[k][1]) == humanSymbol)
                    score -= 8;
            }

            // Corner bonuses for 5x5
            int corners[4][2] = {{0, 0}, {0, 4}, {4, 0}, {4, 4}};
            for (int k = 0; k < 4; k++)
            {
                if (board.getCell(corners[k][0], corners[k][1]) == aiSymbol)
                    score += 5;
                else if (board.getCell(corners[k][0], corners[k][1]) == humanSymbol)
                    score -= 5;
            }
        }

        return score;
    }

    // this function checks how good a specific line (row, column, or diagonal) is for winning
    // it counts AI pieces, human pieces, and empty spaces in that line
    int evaluateLine(const Board &board, int startRow, int startCol, int deltaRow, int deltaCol,
                     char aiSymbol, char humanSymbol, int winCondition) const
    {
        // count how many pieces each player has in this line
        int aiCount = 0, humanCount = 0, emptyCount = 0;

        // go through each position in the line we're checking
        // deltaRow and deltaCol determine the direction:
        // for row: deltaRow=0, deltaCol=1 (horizontal →)
        // for column: deltaRow=1, deltaCol=0 (vertical ↓)
        // for diagonal: deltaRow=1, deltaCol=1 (diagonal ↘) or deltaRow=1, deltaCol=-1 (diagonal ↙)
        for (int i = 0; i < winCondition; i++)
        {
            // calculate the actual position on the board
            int row = startRow + i * deltaRow;
            int col = startCol + i * deltaCol;

            // make sure we're still inside the board boundaries
            if (row >= 0 && row < board.getSize() && col >= 0 && col < board.getSize())
            {
                char cell = board.getCell(row, col);
                if (cell == aiSymbol)
                    aiCount++; // count ai pieces on this
                else if (cell == humanSymbol)
                    humanCount++; // count human pieces on this
                else
                    emptyCount++; // count empty spaces
            }
        }

        // score the line based on how many pieces each player has
        // more pieces in a line = higher score (closer to winning)
        if (aiCount > 0)
        {
            // improve scoring based on how close to winning (winCondition - 1 means one move from win)
            if (aiCount == winCondition - 1)
                return 100; // winning move
            else
                return aiCount * 10; // ai gets positive points: 1 piece = 10 pts, 2 pieces = 20 pts, etc.
        }
        else if (humanCount > 0)
        {
            // blocking is more important when opponent is close to winning
            if (humanCount == winCondition - 1)
                return -200; // must block
            else
                return -humanCount * 15; // human gets points (negative for AI) - we want to block
        }

        return 0; // Empty line has no strategic value
    }

    void getBestMove(const Board &board, int &row, int &col) const
    {
        char aiSymbol = symbol;
        char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';

        // psuedo code 1: try to win or check for win
        for (int i = 0; i < board.getSize(); i++)
        {
            for (int j = 0; j < board.getSize(); j++)
            {
                if (board.isValidMove(i, j))
                {
                    Board testBoard = board;
                    testBoard.makeMove(i, j, aiSymbol);
                    if (testBoard.checkWin(aiSymbol))
                    {
                        row = i;
                        col = j;
                        return; // win now
                    }
                }
            }
        }

        // psuedo code 2: block opponent from winning
        for (int i = 0; i < board.getSize(); i++)
        {
            for (int j = 0; j < board.getSize(); j++)
            {
                if (board.isValidMove(i, j))
                {
                    Board testBoard = board;
                    testBoard.makeMove(i, j, humanSymbol);
                    if (testBoard.checkWin(humanSymbol))
                    {
                        row = i;
                        col = j;
                        return; // block opponent
                    }
                }
            }
        }

        // psuedo code 3: use minimax for better strategy moves
        int bestScore = -1000;
        int bestRow = -1, bestCol = -1;
        for (int i = 0; i < board.getSize(); i++)
        {
            for (int j = 0; j < board.getSize(); j++)
            {
                if (board.isValidMove(i, j))
                {
                    Board tempBoard = board;            // create a board because we passed the board as const
                    tempBoard.makeMove(i, j, aiSymbol); // make the move on the board based on getmove() in AIPlayer

                    // AI just moved, so next turn is human's that's why we pass false - minimize in minimax
                    int score = minimax(tempBoard, 0, false, aiSymbol, humanSymbol);

                    // bonuses based on grid size
                    if (board.getSize() == 4)
                    {
                        // 4x4: Prefer center 2x2 area
                        if ((i >= 1 && i <= 2) && (j >= 1 && j <= 2))
                            score += 3;
                    }
                    else if (board.getSize() == 5)
                    {
                        // 5x5: Prefer center position most, then next to center
                        if (i == 2 && j == 2)
                            score += 5; // center
                        else if (abs(i - 2) <= 1 && abs(j - 2) <= 1)
                            score += 2; // next to center
                    }

                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }

        // no moves found
        if (bestRow == -1 || bestCol == -1)
        {
            getRandomMove(board, row, col);
            return;
        }

        row = bestRow;
        col = bestCol;
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

        // 80% chance to play best move (0-79) - challenging but not impossible
        if (choice < 80)
        {
            getBestMove(board, row, col);
        }
        // 20% chance to play random move (80-99) - gives human opportunities
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
};

class Game
{
private:
    Board *myBoard;
    Player *player1;
    Player *player2;
    int currentPlayer; // 1 or 2

public:
    Game()
    {
        myBoard = new Board();
        myBoard->makeBoard(3);
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
             << "3. Change Board Size" << endl
             << "4. Exit" << endl
             << "Choose a mode: ";
        cin >> choice;
        if (choice == 1)
        {
            setupPVP();
        }
        else if (choice == 2)
        {
            int difficulty;
            setupPVC(difficulty);
        }
        else if (choice == 3)
        {
            setBoardSize();
        }
        else if (choice == 4)
        {
            return;
        }

        startGame();
    }

    void startGame()
    {
        bool gameOver = false;
        bool exit = false;
        int player1Counter = 0, player2Counter = 0;
        Player *current;
        bool restart = false;
        while (!exit)
        {

            myBoard->display();
            while (!gameOver)
            {
                current = (currentPlayer == 1 ? player1 : player2);
                int row, col;

                current->getMove(*myBoard, row, col);
                if (myBoard->makeMove(row, col, current->getSymbol()))
                {
                    if (dynamic_cast<AIPlayer *>(current) != nullptr) // checks if current player is ai or not
                    {
                        cout << endl;
                        cout << "AI made a move at (" << row << ", " << col << ") with symbol '" << current->getSymbol() << "'" << endl;
                    }
                    myBoard->display();
                    if (myBoard->checkWin(current->getSymbol()))
                    {
                        cout << "Player " << current->getName() << " wins!" << endl;
                        (currentPlayer == 1 ? player1Counter++ : player2Counter++);
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
                    cout << "Invalid Cell!" << endl;
                }
            }
            int endChoice;
            cout << "------- Current Score -------" << endl;
            cout << player1Counter << " : " << player2Counter << endl;
            cout << "1. Play another game" << endl
                 << "2.Exit" << endl;
            cin >> endChoice;

            if (endChoice == 2)
            {
                exit = true;
                restart = false;
            }
            else if (endChoice == 1)
            {
                restart = true;
                myBoard->reset();
                if (dynamic_cast<AIPlayer *>(player2) != nullptr) // checks if current player is ai or not
                {
                    int choice;
                    cout << "1. Change the difficulty" << endl
                         << "2. Continue" << endl;
                    cin >> choice;
                    if (choice == 1)
                    {
                        int newDifficulty;
                        cout << "------ Playing another game ------" << endl
                             << "Choose your difficulty! " << endl
                             << "1. Easy" << endl
                             << "2. Medium" << endl
                             << "3. Hard" << endl;
                        cin >> newDifficulty;

                        // set difficulty for the AI player
                        if (dynamic_cast<AIPlayer *>(player2) != nullptr)
                        {
                            player2->setDifficulty(newDifficulty);
                        }
                        gameOver = false;
                    }
                }
            }
        }
        if (!restart)
        {
            myBoard->reset();
            showMenu();
        }
    }
    void setupPVP()
    {
        // configures player vs player mode with user input
        cin.ignore();
        string name1;
        cout << "Enter Player 1 Name: " << endl;
        getline(cin, name1);
        player1 = new HumanPlayer(name1, 'X');

        string name2;
        cout << "Enter Player 2 Name: " << endl;
        getline(cin, name2);
        player2 = new HumanPlayer(name2, 'O');
    }

    void setupPVC(int &difficulty)
    {
        // configures player vs computer mode with user input
        cin.ignore();
        string name1;
        cout << "Enter Player 1 Name: ";
        getline(cin, name1);
        cout << endl;
        player1 = new HumanPlayer(name1, 'X');
        cout << "Choose your difficulty! " << endl
             << "1. Easy" << endl
             << "2. Medium" << endl
             << "3. Hard" << endl;
        cin >> difficulty;

        player2 = new AIPlayer("Intelligence", 'O', difficulty);
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
    /*
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
    */

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

    void setBoardSize()
    {
        int size;
        bool selectingSize = true;
        while (selectingSize)
        {
            cout << "Choose grid size:" << endl;
            cout << "3. 3x3 grid (classic)" << endl;
            cout << "4. 4x4 grid (intermediate) (need 4 symbols to win)" << endl;
            cout << "5. 5x5 grid (advanced) (need 4 symbols to win)" << endl;
            cout << "Enter your choice (3, 4, or 5): ";
            cin >> size;
            if (size == 3 || size == 4 || size == 5)
            {
                selectingSize = false;
                myBoard->makeBoard(size);
                cout << "Grid size set to " << size << "x" << size << "!" << endl;
            }
            else
            {
                cout << "Invalid choice! Please enter 3, 4, or 5 only." << endl;
            }
        }
        showMenu();
    }

    void reset()
    {
        // prepares game for new round
        myBoard->reset();
        currentPlayer = 1;
    }
    ~Game()
    {
        delete myBoard;
        delete player1;
        delete player2;
    }
};

int main()
{
    Game *mygame = new Game();
    mygame->showMenu();

    delete mygame; // clean memory
    return 0;
}
