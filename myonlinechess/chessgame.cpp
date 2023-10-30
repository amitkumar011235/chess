#include "chessgame.hpp"

void Game::InitializeGame() {
	gb.DisplayGameBoard();

}
bool Game::MakeMove() {
	
	std::string to_record;

	// Get user input for the piece they want to move
	cout << "Choose piece to be moved. (example: A1 or b2): ";

	std::string move_from;
	getline(cin, move_from);

	if (move_from.length() > 2)
	{
		cout<<"You should type only two characters (column and row)\n";
		return false;
	}

	if (toupper(move_from[0]) < 'A' || toupper(move_from[0]) > 'H')
	{
		cout<<"Invalid column.\n";
		return false;
	}

	if (move_from[1] < '0' || move_from[0] > '8')
	{
		cout<<"Invalid row.\n";
		return false;
	}
	Position pos1(move_from[1], move_from[0]);

	// Convert column from ['A'-'H'] to [0x00-0x07]
	pos1.m_col = pos1.m_col - 'A';

	// Convert row from ['1'-'8'] to [0x00-0x07]
	pos1.m_row = pos1.m_row - '1';

	char chpiece = gb.GetPieceAtPosition(pos1.m_row, pos1.m_col);
	cout << "Piece is " << char(chpiece) << "\n";

	if (0x20 == chpiece)
	{
		cout<<"You picked an EMPTY square.\n";
		return false;
	}

	if (PieceColor::WHITE_PIECE == turn)
	{
		if (false == IsWhitePiece(chpiece))
		{
			cout<<"It is WHITE's turn and you picked a BLACK piece\n";
			return false;
		}
	}
	else
	{
		if (false == IsBlackPiece(chpiece))
		{
			cout<<"It is BLACK's turn and you picked a WHITE piece\n";
			return false;
		}
	}


	//-------------------------------------------------------//
	//now input the destination square
	cout << "Move to: ";
	std::string move_to;
	getline(cin, move_to);

	if (move_to.length() > 2)
	{
		cout<<"You should type only two characters (column and row)\n";
		return false;
	}

	// ---------------------------------------------------
	// Did the user pick a valid house to move?
	// Must check if:
	// - It's inside the board (A1-H8)
	// - The move is valid
	// ---------------------------------------------------
	Position future(move_to[1], move_to[0]);
	
	future.m_col = toupper(future.m_col);

	if (future.m_col < 'A' || future.m_col > 'H')
	{
		cout<<"Invalid column.\n";
		return false;
	}

	if (future.m_row < '0' || future.m_row > '8')
	{
		cout<<"Invalid row.\n";
		return false;
	}


	// Convert columns from ['A'-'H'] to [0x00-0x07]
	future.m_col = future.m_col - 'A';

	// Convert row from ['1'-'8'] to [0x00-0x07]
	future.m_row = future.m_row - '1';

	// Check if it is not the exact same square
	if (future.m_row == pos1.m_row && future.m_col == pos1.m_col)
	{
		cout<<"[Invalid] You picked the same square!\n";
		return false;
	}

	// Is that move allowed?
	EnPassant  S_enPassant = { 0 };
	Castling   S_castling = { 0 };
	Promotion  S_promotion = { 0 };

	if (false == IsMoveValid(pos1, future, &S_enPassant, &S_castling, &S_promotion))
	{
		cout<<"[Invalid] Piece can not move to that square!\n";
		return;
	}




	return true;

}

bool Game::IsMoveValid(Position present, Position future, EnPassant* S_enPassant, Castling* S_castling, Promotion* S_promotion)
{
    bool bValid = false;

    char chPiece = gb.GetPieceAtPosition(present.m_row, present.m_col);

    // ----------------------------------------------------
    // 1. Is the piece  allowed to move in that direction?
    // ----------------------------------------------------
    switch (toupper(chPiece))
    {
    case 'P':
    {
        // Wants to move forward
        if (future.m_col == present.m_col)
        {
            // Simple move forward
            if ((IsWhitePiece(chPiece) && future.m_row == present.m_row + 1) ||
                (IsBlackPiece(chPiece) && future.m_row == present.m_row - 1))
            {
                if (EMPTY_SQUARE == gb.GetPieceAtPosition(future.m_row, future.m_col))
                {
                    bValid = true;
                }
            }

            // Double move forward
            else if ((IsWhitePiece(chPiece) && future.m_row == present.m_row + 2) ||
                (IsBlackPiece(chPiece) && future.m_row == present.m_row - 2))
            {
                // This is only allowed if the pawn is in its original place
                if (IsWhitePiece(chPiece))
                {
                    if (EMPTY_SQUARE == gb.GetPieceAtPosition(future.m_row - 1, future.m_col) &&
                        EMPTY_SQUARE == gb.GetPieceAtPosition(future.m_row, future.m_col) &&
                        1 == present.m_row)
                    {
                        bValid = true;
                    }
                }
                else // if ( isBlackPiece(chPiece) )
                {
                    if (EMPTY_SQUARE == gb.GetPieceAtPosition(future.m_row + 1, future.m_col) &&
                        EMPTY_SQUARE == gb.GetPieceAtPosition(future.m_row, future.m_col) &&
                        6 == present.m_row)
                    {
                        bValid = true;
                    }
                }
            }
            else
            {
                // This is invalid
                return false;
            }
        }

        // The "en passant" move
        else if ((IsWhitePiece(chPiece) && 4 == present.m_row && 5 == future.m_row && 1 == abs(future.m_col - present.m_col)) ||
            (IsBlackPiece(chPiece) && 3 == present.m_row && 2 == future.m_row && 1 == abs(future.m_col - present.m_col)))
        {
            // It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
            string last_move = current_game->getLastMove();

            // Parse the line
            Position LastMoveFrom;
            Position LastMoveTo;
            current_game->parseMove(last_move, &LastMoveFrom, &LastMoveTo);

            // First of all, was it a pawn?
            char chLstMvPiece = current_game->getPieceAtPosition(LastMoveTo.m_row, LastMoveTo.m_col);

            if (toupper(chLstMvPiece) != 'P')
            {
                return false;
            }

            // Did the pawn have a double move forward and was it an adjacent column?
            if (2 == abs(LastMoveTo.m_row - LastMoveFrom.m_row) && 1 == abs(LastMoveFrom.m_col - present.m_col))
            {
                cout << "En passant move!\n";
                bValid = true;

                S_enPassant->bApplied = true;
                S_enPassant->PawnCaptured.m_row = LastMoveTo.m_row;
                S_enPassant->PawnCaptured.m_col = LastMoveTo.m_col;
            }
        }

        // Wants to capture a piece
        else if (1 == abs(future.m_col - present.m_col))
        {
            if ((IsWhitePiece(chPiece) && future.m_row == present.m_row + 1) || (IsBlackPiece(chPiece) && future.m_row == present.m_row - 1))
            {
                // Only allowed if there is something to be captured in the square
                if (EMPTY_SQUARE != gb.GetPieceAtPosition(future.m_row, future.m_col))
                {
                    bValid = true;
                    cout << "Pawn captured a piece!\n";
                }
            }
        }
        else
        {
            // This is invalid
            return false;
        }

        // If a pawn reaches its eight rank, it must be promoted to another piece
        if ((IsWhitePiece(chPiece) && 7 == future.m_row) ||
            (IsBlackPiece(chPiece) && 0 == future.m_row))
        {
            cout << "Pawn must be promoted!\n";
            S_promotion->bApplied = true;
        }
    }
    break;

    case 'R':
    {
        // Horizontal move
        if ((future.m_row == present.m_row) && (future.m_col != present.m_col))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Direction::HORIZONTAL))
            {
                bValid = true;
            }
        }
        // Vertical move
        else if ((future.m_row != present.m_row) && (future.m_col == present.m_col))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Direction::VERTICAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'N':
    {
        if ((2 == abs(future.m_row - present.m_row)) && (1 == abs(future.m_col - present.m_col)))
        {
            bValid = true;
        }

        else if ((1 == abs(future.m_row - present.m_row)) && (2 == abs(future.m_col - present.m_col)))
        {
            bValid = true;
        }
    }
    break;

    case 'B':
    {
        // Diagonal move
        if (abs(future.m_row - present.m_row) == abs(future.m_col - present.m_col))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Direction::DIAGONAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'Q':
    {
        // Horizontal move
        if ((future.m_row == present.m_row) && (future.m_col != present.m_col))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Direction::HORIZONTAL))
            {
                bValid = true;
            }
        }
        // Vertical move
        else if ((future.m_row != present.m_row) && (future.m_col == present.m_col))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::VERTICAL))
            {
                bValid = true;
            }
        }

        // Diagonal move
        else if (abs(future.m_row - present.m_row) == abs(future.m_col - present.m_col))
        {
            // Check if there are no pieces on the way
            if (current_game->isPathFree(present, future, Chess::DIAGONAL))
            {
                bValid = true;
            }
        }
    }
    break;

    case 'K':
    {
        // Horizontal move by 1
        if ((future.m_row == present.m_row) && (1 == abs(future.m_col - present.m_col)))
        {
            bValid = true;
        }

        // Vertical move by 1
        else if ((future.m_col == present.m_col) && (1 == abs(future.m_row - present.m_row)))
        {
            bValid = true;
        }

        // Diagonal move by 1
        else if ((1 == abs(future.m_row - present.m_row)) && (1 == abs(future.m_col - present.m_col)))
        {
            bValid = true;
        }

        // Castling
        else if ((future.m_row == present.m_row) && (2 == abs(future.m_col - present.m_col)))
        {
            // Castling is only allowed in these circunstances:

            // 1. King is not in check
            if (true == current_game->playerKingInCheck())
            {
                return false;
            }

            // 2. No pieces in between the king and the rook
            if (false == current_game->isPathFree(present, future, Direction::HORIZONTAL))
            {
                return false;
            }

            // 3. King and rook must not have moved yet;
            // 4. King must not pass through a square that is attacked by an enemy piece
            if (future.m_col > present.m_col)
            {
                // if future.m_col is greather, it means king side
                if (false == current_game->castlingAllowed(Side::KING_SIDE, GetPieceColor(chPiece)))
                {
                    cout<<"Castling to the king side is not allowed.\n";
                    return false;
                }
                else
                {
                    // Check if the square that the king skips is not under attack
                    UnderAttack square_skipped = current_game->isUnderAttack(present.m_row, present.m_col + 1, current_game->getCurrentTurn());
                    if (false == square_skipped.bUnderAttack)
                    {
                        // Fill the S_castling structure
                        S_castling->bApplied = true;

                        // Present position of the rook
                        S_castling->rook_before.m_row = present.m_row;
                        S_castling->rook_before.m_col = present.m_col + 3;

                        // Future position of the rook
                        S_castling->rook_after.m_row = future.m_row;
                        S_castling->rook_after.m_col = present.m_col + 1; // future.m_col -1

                        bValid = true;
                    }
                }
            }
            else //if (future.m_col < present.m_col)
            {
                // if present.m_col is greather, it means queen side
                if (false == current_game->castlingAllowed(Chess::Side::QUEEN_SIDE, GetPieceColor(chPiece)))
                {
                    cout<<"Castling to the queen side is not allowed.\n";
                    return false;
                }
                else
                {
                    // Check if the square that the king skips is not attacked
                    UnderAttack square_skipped = current_game->isUnderAttack(present.m_row, present.m_col - 1, current_game->getCurrentTurn());
                    if (false == square_skipped.bUnderAttack)
                    {
                        // Fill the S_castling structure
                        S_castling->bApplied = true;

                        // Present position of the rook
                        S_castling->rook_before.m_row = present.m_row;
                        S_castling->rook_before.m_col = present.m_col - 4;

                        // Future position of the rook
                        S_castling->rook_after.m_row = future.m_row;
                        S_castling->rook_after.m_col = present.m_col - 1; // future.m_col +1

                        bValid = true;
                    }
                }
            }
        }
    }
    break;

    default:
    {
        cout << "!!!!Should not reach here. Invalid piece: " << char(chPiece) << "\n\n\n";
    }
    break;
    }

    // If it is a move in an invalid direction, do not even bother to check the rest
    if (false == bValid)
    {
        cout << "Piece is not allowed to move to that square\n";
        return false;
    }


    // -------------------------------------------------------------------------
    // 2. Is there another piece of the same color on the destination square?
    // -------------------------------------------------------------------------
    if (current_game->isSquareOccupied(future.m_row, future.m_col))
    {
        char chAuxPiece = current_game->getPieceAtPosition(future.m_row, future.m_col);
        if (GetPieceColor(chPiece) == GetPieceColor(chAuxPiece))
        {
            cout << "Position is already taken by a piece of the same color\n";
            return false;
        }
    }

    // ----------------------------------------------
    // 3. Would the king be in check after the move?
    // ----------------------------------------------
    if (true == current_game->wouldKingBeInCheck(chPiece, present, future, S_enPassant))
    {
        cout << "Move would put player's king in check\n";
        return false;
    }

    return bValid;
}


int Game::GetPieceColor(char chPiece)
{
	if (isupper(chPiece))
	{
		return WHITE_PIECE;
	}
	else
	{
		return BLACK_PIECE;
	}
}


bool Game::IsWhitePiece(char chPiece)
{
	return GetPieceColor(chPiece) == PieceColor::WHITE_PIECE ? true : false;
}

bool Game::IsBlackPiece(char chPiece)
{
	return GetPieceColor(chPiece) == PieceColor::BLACK_PIECE ? true : false;
}

string Game::DescribePiece(char chPiece)
{
	std::string description;

	if (IsWhitePiece(chPiece))
	{
		description += "White ";
	}
	else
	{
		description += "Black ";
	}

	switch (toupper(chPiece))
	{
	case 'P':
	{
		description += "pawn";
	}
	break;

	case 'N':
	{
		description += "knight";
	}
	break;

	case 'B':
	{
		description += "bishop";
	}
	break;

	case 'R':
	{
		description += "rook";
	}
	break;

	case 'Q':
	{
		description += "queen";
	}
	break;

	default:
	{
		description += "unknow piece";
	}
	break;
	}

	return description;
}

string Game::GetLastMove(void)
{
    string last_move;

    // Who did the last move?
    if (BLACK_PLAYER == turn)
    {
        // If it's black's turn now, white had the last move
        last_move = rounds[rounds.size() - 1].white_move;
    }
    else
    {
        // Last move was black's
        last_move = rounds[rounds.size() - 1].black_move;
    }

    return last_move;
}

bool Game::IsPathFree(Position startingPos, Position finishingPos, int iDirection)
{
    bool bFree = false;

    switch (iDirection)
    {
    case Direction::HORIZONTAL:
    {
        // If it is a horizontal move, we can assume the startingPos.m_row == finishingPos.m_row
        // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
        if (startingPos.m_col == finishingPos.m_col)
        {
            cout << "Error. Movement is horizontal but column is the same\n";
        }

        // Moving to the right
        else if (startingPos.m_col < finishingPos.m_col)
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = startingPos.m_col + 1; i < finishingPos.m_col; i++)
            {
                if (IsSquareOccupied(startingPos.m_row, i))
                {
                    bFree = false;
                    cout << "Horizontal path to the right is not clear!\n";
                }
            }
        }

        // Moving to the left
        else //if (startingPos.m_col > finishingPos.m_col)
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = startingPos.m_col - 1; i > finishingPos.m_col; i--)
            {
                if (IsSquareOccupied(startingPos.m_row, i))
                {
                    bFree = false;
                    cout << "Horizontal path to the left is not clear!\n";
                }
            }
        }
    }
    break;

    case Direction::VERTICAL:
    {
        // If it is a vertical move, we can assume the startingPos.m_col == finishingPos.m_col
        // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
        if (startingPos.m_row == finishingPos.m_row)
        {
            cout << "Error. Movement is vertical but row is the same\n";
            throw("Error. Movement is vertical but row is the same");
        }

        // Moving up
        else if (startingPos.m_row < finishingPos.m_row)
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = startingPos.m_row + 1; i < finishingPos.m_row; i++)
            {
                if (IsSquareOccupied(i, startingPos.m_col))
                {
                    bFree = false;
                    cout << "Vertical path up is not clear!\n";
                }
            }
        }

        // Moving down
        else //if (startingPos.m_col > finishingPos.m_row)
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = startingPos.m_row - 1; i > finishingPos.m_row; i--)
            {
                if (IsSquareOccupied(i, startingPos.m_col))
                {
                    bFree = false;
                    cout << "Vertical path down is not clear!\n";
                }
            }
        }
    }
    break;

    case Direction::DIAGONAL:
    {
        // Moving up and right
        if ((finishingPos.m_row > startingPos.m_row) && (finishingPos.m_col > startingPos.m_col))
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = 1; i < abs(finishingPos.m_row - startingPos.m_row); i++)
            {
                if (IsSquareOccupied(startingPos.m_row + i, startingPos.m_col + i))
                {
                    bFree = false;
                    cout << "Diagonal path up-right is not clear!\n";
                }
            }
        }

        // Moving up and left
        else if ((finishingPos.m_row > startingPos.m_row) && (finishingPos.m_col < startingPos.m_col))
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = 1; i < abs(finishingPos.m_row - startingPos.m_row); i++)
            {
                if (IsSquareOccupied(startingPos.m_row + i, startingPos.m_col - i))
                {
                    bFree = false;
                    cout << "Diagonal path up-left is not clear!\n";
                }
            }
        }

        // Moving down and right
        else if ((finishingPos.m_row < startingPos.m_row) && (finishingPos.m_col > startingPos.m_col))
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = 1; i < abs(finishingPos.m_row - startingPos.m_row); i++)
            {
                if (IsSquareOccupied(startingPos.m_row - i, startingPos.m_col + i))
                {
                    bFree = false;
                    cout << "Diagonal path down-right is not clear!\n";
                }
            }
        }

        // Moving down and left
        else if ((finishingPos.m_row < startingPos.m_row) && (finishingPos.m_col < startingPos.m_col))
        {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = 1; i < abs(finishingPos.m_row - startingPos.m_row); i++)
            {
                if (isSquareOccupied(startingPos.m_row - i, startingPos.m_col - i))
                {
                    bFree = false;
                    cout << "Diagonal path down-left is not clear!\n";
                }
            }
        }

        else
        {
            throw("Error. Diagonal move not allowed");
        }
    }
    break;
    }

    return bFree;
}


bool Game::IsSquareOccupied(int iRow, int iColumn)
{
    bool bOccupied = false;

    if (0x20 != gb.GetPieceAtPosition(iRow, iColumn))
    {
        bOccupied = true;
    }

    return bOccupied;
}




