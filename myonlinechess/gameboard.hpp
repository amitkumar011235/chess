#pragma once

#include "common.hpp"


class GameBoard {
private:
	const int ROWS = 8;
	const int COLUMNS = 8;
	vector<vector<char>> matrix;
	std::unordered_map<char, std::string> chessSymbols;
	void PrintLine(int iLine, int iColor1, int iColor2);

	// Save the captured pieces
	std::vector<char> white_captured;
	std::vector<char> black_captured;


public:
	GameBoard();
	void DisplayGameBoard();
	void DisplayChessboard(const std::vector<std::string>& board);
	bool UpdateBoard(Position initPosition, Position finalPosition);
	int  GetPieceColor(char piece);
	char GetPieceAtPosition(int row, int col);

};