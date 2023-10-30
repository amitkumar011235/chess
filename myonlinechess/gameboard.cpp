#include "gameboard.hpp"

GameBoard::GameBoard() {
    //black pieces
    chessSymbols['p'] = 'p';// u8"\u265F";  // Black pawn
    chessSymbols['r'] = 'r';// u8"\u265C";  // Black rook
    chessSymbols['n'] = 'n';// u8"\u265E";  // Black knight
    chessSymbols['b'] = 'b';// u8"\u265D";  // Black bishop
    chessSymbols['q'] = 'q';// u8"\u265B";  // Black queen
    chessSymbols['k'] = 'k';// u8"\u265A";  // Black king

    // White pieces
    chessSymbols['P'] = 'P';// u8"\u2659";  // White pawn
    chessSymbols['R'] = 'R';// u8"\u2656";  // White rook
    chessSymbols['N'] = 'N';// u8"\u2658";  // White knight
    chessSymbols['B'] = 'B';// u8"\u2657";  // White bishop
    chessSymbols['Q'] = 'Q';// u8"\u2655";  // White queen
    chessSymbols['K'] = 'K';// u8"\u2654";  // White king

    //initialize the chess board matrix with the initial game setup
    matrix.resize(8, std::vector<char>(8, ' '));
    matrix =
    {
        // This represents the pieces on the board.
        // Keep in mind that pieces[0][0] represents A1
        // pieces[1][1] represents B2 and so on.
        // Letters in CAPITAL are white
        { 'R',  'N',  'B',  'Q',  'K',  'B',  'N',  'R' },
        { 'P',  'P',  'P',  'P',  'P',  'P',  'P',  'P' },
        { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
        { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
        { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
        { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 },
        { 'p',  'p',  'p',  'p',  'p',  'p',  'p',  'p' },
        { 'r',  'n',  'b',  'q',  'k',  'b',  'n',  'r' },
    };

}

void GameBoard::PrintLine(int iLine, int iColor1, int iColor2) {
    // Example (for CELL = 6):

   //  [6-char]
   //  |______| subline 1
   //  |___Q__| subline 2
   //  |______| subline 3

   // Define the CELL variable here. 
   // It represents how many horizontal characters will form one square
   // The number of vertical characters will be CELL/2
   // You can change it to alter the size of the board (an odd number will make the squares look rectangular)
    int CELL = 6;

    // Since the width of the characters BLACK and WHITE is half of the height,
    // we need to use two characters in a row.
    // So if we have CELL characters, we must have CELL/2 sublines
    for (int subLine = 0; subLine < CELL / 2; subLine++)
    {
        // A sub-line is consisted of 8 cells, but we can group it
        // in 4 iPairs of black&white
        for (int iPair = 0; iPair < 4; iPair++)
        {
            // First cell of the pair
            for (int subColumn = 0; subColumn < CELL; subColumn++)
            {
                // The piece should be in 
                // the "middle" of the cell
                // For 3 sub-lines, in sub-line 1
                // For 6 sub-columns, sub-column 3
                if (subLine == 1 && subColumn == 3)
                {
                    cout << char(matrix[iLine][iPair * 2] != 0x20 ? matrix[iLine][iPair * 2] : iColor1);
                }
                else
                {
                    cout << char(iColor1);
                }
            }

            // Second cell of the pair
            for (int subColumn = 0; subColumn < CELL; subColumn++)
            {
                // The piece should be in the "middle" of the cell
                // For 3 sub-lines, in sub-line 1
                // For 6 sub-columns, sub-column 3
                if (subLine == 1 && subColumn == 3)
                {
                    cout << char(matrix[iLine][iPair*2 + 1] != 0x20 ? matrix[iLine][iPair*2 + 1]: iColor2);
                }
                else
                {
                    cout << char(iColor2);
                }
            }
        }

        // Write the number of the line on the right
        if (1 == subLine)
        {
            cout << "   " << iLine + 1;
        }

        cout << "\n";

    }
}

void GameBoard::DisplayChessboard(const std::vector<std::string>& board) {
    cout << "   A     B     C     D     E     F     G     H\n\n";

    for (int iLine = 7; iLine >= 0; iLine--)
    {
        if (iLine % 2 == 0)
        {
            // Line starting with BLACK
            PrintLine(iLine, BLACK_SQUARE, WHITE_SQUARE);
        }

        else
        {
            // Line starting with WHITE
            PrintLine(iLine, WHITE_SQUARE, BLACK_SQUARE);
        }
    }
}

void GameBoard::DisplayGameBoard() {
    std::vector<std::string> chessboard = {
       "rnbqkbnr", // Black back rank
       "pppppppp", // Black pawns
       "        ", // Empty ranks
       "        ",
       "        ",
       "        ",
       "PPPPPPPP", // White pawns
       "RNBQKBNR"  // White back rank
    };

    // Display the chessboard
    DisplayChessboard(chessboard);
}

bool GameBoard::UpdateBoard(Position initPosition, Position finalPosition) {
    if (matrix[initPosition.m_row][initPosition.m_col] == 0x20) {
        return false;
    }
    return true;

}

int GameBoard::GetPieceColor(char piece)
{
    if (isupper(piece))
    {
        return WHITE_PIECE;
    }
    else
    {
        return BLACK_PIECE;
    }
}

char GameBoard::GetPieceAtPosition(int row, int col) {
    matrix[row][col];
}

