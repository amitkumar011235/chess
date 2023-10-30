#pragma once

#include "common.hpp"
#include "gameboard.hpp"


class Game {
private:
	string user_white;
	string user_black;
	GameBoard gb;
	struct Round
	{
		string white_move;
		string black_move;
	};

	//std::deque<std::string> moves;
	std::deque<Round> rounds;
	int turn = 0; // 0 means white turn and 1 means black turn

	bool IsValidMove(Position initPosition, Position finalPosition);



public:
	void	InitializeGame();
	bool	MakeMove();
	int		GetPieceColor(char chPiece);
	bool	IsWhitePiece(char chPiece);
	bool	IsBlackPiece(char chPiece);
	string  DescribePiece(char chPiece);
	bool	IsMoveValid(Position present, Position future, EnPassant* S_enPassant,								Castling* S_castling, Promotion* S_promotion);
	string	GetLastMove(void);
	bool	IsPathFree(Position startingPos, Position finishingPos, int iDirection);
	bool	IsSquareOccupied(int iRow, int iColumn);








};