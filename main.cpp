#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

// ================== Board Class ==================
class Board {
private:
    vector<vector<char>> grid;
    int size;

public:
    Board(int s = 3);

    void display() const;

    bool makeMove(int row, int col, char symbol);

    bool isValidMove(int row, int col) const;

    bool checkWin(char symbol) const;

    bool isFull() const;

    char getCell(int row, int col) const;

    void reset();

    int getSize() const;
};

// ================== Player (Abstract) ==================
class Player {
protected:
    string name;
    char symbol;
public:
    Player(const string &name, char symbol);

    virtual void getMove(int &row, int &col, const Board &board) = 0;

    string getName() const;

    char getSymbol() const;

    void setName(const string &n);
};

// ================== Human Player ==================
class HumanPlayer : public Player {
public:
    HumanPlayer(const string &name, char symbol);

    void getMove(int &row, int &col, const Board &board) override;
};

// ================== AI Player ==================
class AIPlayer : public Player {
private:
    int difficulty;

    void getRandomMove(const Board &board, int &row, int &col) const;

    int minimax(Board &board, bool isMax) const;

    void getBestMove(Board &board, int &row, int &col) const;

public:
    AIPlayer(const string &name, char symbol, int diff);

    void setDifficulty(int d);

    void getMove(int &row, int &col, const Board &board) override;
};

// ================== Game Class ==================
class Game {
private:
    Board board;
    Player *player1;
    Player *player2;
    Player *current;

public:
    Game();

    void showMenu();

    void setupPvP();

    void setupPVC(int diff);

    void switchPlayer();

    bool checkGameEnd();

    void start();
};

// ================== Main ==================
int main() {
    Game g;
    g.start();
    return 0;
}
