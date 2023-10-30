#pragma once

#include "common.hpp"

class ChessManager {

private:
	void clearScreen();
	void showLogo();
	void showMenu();
	void startGame();
	void inGameMenu();

public:
	void StartGameApplication();
};