#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
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
        return true;
    }

    bool isValidMove(int row, int col) const
    {
        // checks if this is a valid spot to put a X or O
        return true;
    }

    bool checkWin(char symbol) const
    {
        // checks the win conditions (rows, columns , diagonals)
        return false;
    }

    bool isFull() const
    {
        // indicating board full status
        return false;
    }

    char getCell(int row, int col) const
    {
        // return symbol at specified coordinates
        return grid[row][col];
    }
    void reset()
    {
        // clears all cells to empty state
    }
    int getSize()
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

    virtual void getMove(const Board &myBoard, int &row, int &col) = 0;
    // this will be overriden in the next classes for
    // human player
    // ai player for each one we will use a different getMove function

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
    string name;
    char symbol;
    int difficulty;

public:
    // ": Player(n, sym)" makes a para. constructor for Player
    AIPlayer(const string &n = "Sama7", char sym = 'A', int diff = 1) : Player(n, sym)
    {
        name = n;
        symbol = sym;
        difficulty = diff;
    }
    void setDifficulty(int newDifficulty) // 1.easy 2. medium 3. hard
    {
        difficulty = newDifficulty;
    }

    void getRandomMove(const Board &board, int &row, int &col) const
    {
        // selects random valid move for easy difficulty
    }

    void getBestMove(const Board &board, int &row, int &col) const
    {
        // does the best move : if the player is going to win :
        // put my symbol in the winning move
        // if the ai is going to win:
        // put the symbol in the winning position
    }

    void getGoodMove(const Board &board, int &row, int &col) const
    {
        // sometimes plays best moves and sometimes plays random moves for medium difficulty
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

public:
    Game()
    {
        /*int size;
        cout << "What is the size of your grid?" << endl;
        cin >> size;
        لسة مش عارف هنعمل فيها ايه بس دي ك بداية*/
        myBoard = new Board(); // Allocate memory first!
        myBoard->makeBoard(3); // Then initialize with size 3
        player1 = nullptr;
        player2 = nullptr;
        currentPlayer = 1;
    }
    void startGame()
    {
        // Main game entry point
        /* cout << "Player" << currentPlayer << " start!";
        maybe */
    }
    void showMenu()
    {
        // dispplayes mode selection menu and handles user choices
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
        // shows game outcome message
        // Player1 Wins!
    }
    void reset()
    {
        // prepares game for new round
    }
    ~Game()
    {
        delete myBoard;
        delete player1;
        delete player2;
    }
};
int testing()
{
    Board myboard;
    myboard.makeBoard();
    myboard.display();
    return 0;
}
int main()
{
    testing();
    /*
    // تقريبا
    Game myGame;
    myGame.showMenu();
    return 0;
    */
}