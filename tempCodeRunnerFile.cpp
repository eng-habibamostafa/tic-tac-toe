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

                // Check if human player wants to quit
                if (dynamic_cast<HumanPlayer *>(current) != nullptr && row == -1 && col == -1)
                {
                    cout << "\nGame ended by player " << current->getName() << ". Returning to main menu...\n"
                         << endl;
                    gameOver = true;
                    exit = true;
                    break;
                }

                if (dynamic_cast<HumanPlayer *>(current) != nullptr) // if human player
                {
                    row--; // make input 1-indexed
                    col--;
                }
                // AI players use 0-indexed

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
                gameOver = false;
                myBoard->reset();
                if (dynamic_cast<AIPlayer *>(player2) != nullptr) // checks if current player is ai or not
                {
                    int choice;
                    cout << "---------- Settings ----------" << endl
                         << "1. Continue" << endl
                         << "2. Change the difficulty" << endl;
                    cin >> choice;
                    if (choice == 2)
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