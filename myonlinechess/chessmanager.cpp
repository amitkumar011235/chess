#include "chessmanager.hpp" 
#include "chessgame.hpp"

// Function to clear the console screen
void ChessManager::clearScreen() {
    // Implementation for clearing the console screen (platform-dependent)
    // For example, on Windows, you can use the "cls" command:
    system("cls");
    // On Unix-based systems (Linux/Mac), you can use "clear":
    // system("clear");
}

// Function to display a logo or game title
void ChessManager::showLogo() {
    clearScreen();
    std::cout << "=== Chess Game ===" << std::endl;
    // You can add more to the logo if desired
}

// Function to display the main menu
void ChessManager::showMenu() {
    showLogo();
    std::cout << "1. Start New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

// Function to start a new game
void ChessManager::startGame() {
    // Implement game initialization logic here
    // Create a new chessboard, set up pieces, etc.
}

// Function to display an in-game menu
void ChessManager::inGameMenu() {
    clearScreen();
    std::cout << "=== In-Game Menu ===" << std::endl;
    std::cout << "1. Save Game" << std::endl;
    std::cout << "2. Resume Game" << std::endl;
    std::cout << "3. Quit to Main Menu" << std::endl;
}

void ChessManager::StartGameApplication() {
    showMenu();
    int option;


    while (1) {
       // SetConsoleOutputCP(CP_UTF8);
       cin >> option;
       switch (option) {
       case 1:
       {
           cout << "initiating new game..." << endl;
           Game game;
           game.InitializeGame();
           game.MakeMove();
       }

           break;

       case 2:
           cout << "loading game from file..." << endl;
           break;

       case 3:
           cout << "exiting the game..." << endl;
           exit(1);
           break;

       default:
           cout << "Unknown option. Please select the correct option." << endl;
           //showMenu();
           break;
       }

    }
}


