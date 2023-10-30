#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <deque>

using namespace std;

#define BLACK_SQUARE 0xFF
#define WHITE_SQUARE 0xDB
#define EMPTY_SQUARE 0x20


class Position {
public:
	int m_row;
	int m_col;
    Position(){}

	Position(int row, int col) :m_row(row), m_col(col) {}
};

enum PieceColor
{
    WHITE_PIECE = 0,
    BLACK_PIECE = 1
};

enum Player
{
    WHITE_PLAYER = 0,
    BLACK_PLAYER = 1
};

enum Side
{
    QUEEN_SIDE = 2,
    KING_SIDE = 3
};

enum Direction
{
    HORIZONTAL = 0,
    VERTICAL,
    DIAGONAL,
    L_SHAPE
};

struct EnPassant
{
    bool bApplied;
    Position PawnCaptured;
};

struct Castling
{
    bool bApplied;
    Position rook_before;
    Position rook_after;
};

struct Promotion
{
    bool bApplied;
    //Position  pos;
    char chBefore;
    char chAfter;
};

struct IntendedMove
{
    char chPiece;
    Position from;
    Position to;
};

struct Attacker
{
    Position  pos;
    Direction dir;
};

struct UnderAttack
{
    bool bUnderAttack;
    int iNumAttackers;
    Attacker attacker[9]; //maximum theorical number of attackers
};

