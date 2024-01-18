#include "chessboard.h"
#include "qdebug.h"

ChessBoard::ChessBoard(QObject *parent)
    : QObject{parent}
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            board[i][j] = nullptr;
        }
    }
}

ChessBoard::~ChessBoard()
{

}

ChessPiece* ChessBoard::getPiece(const Position& position) const
{
    if (position.row >= 1 && position.row <= 8 && position.column >= 1 && position.column <= 8)
    {
        return board[position.row][position.column];
    }
    return nullptr;
}

void ChessBoard::setPiece(const Position& position, ChessPiece* piece)
{
    if (position.row >= 1 && position.row <= 8 && position.column >= 1 && position.column <= 8)
    {
        board[position.row][position.column] = piece;
    }
    else
    {
        //qDebug() << "Not allowed piece location!";
    }
}

void ChessBoard::print()
{
    for (int i = 8; i >= 1; i--)
    {
        QString rowString = " ";

        for (int j = 1; j <= 8; j++)
        {
            if (board[i][j])
            {
                ChessPiece* piece = board[i][j];
                QChar rowChar = piece->type()[0];

                rowString += piece->isWhite() ? rowChar.toUpper() : rowChar.toLower();
            }
            else
            {
                rowString += '.';
            }
            rowString += ' ';
        }

        qDebug() << rowString;
    }
}

bool ChessBoard::pieceAlreadyAt(const Position& position)
{
    return board[position.row][position.column] != nullptr;
}

std::vector<ChessPiece *> ChessBoard::getPlayerPieces(bool isWhite)
{
    std::vector<ChessPiece*> pieces;
    for (int i = 8; i >= 1; i--)
    {
        for (int j = 1; j <= 8; j++)
        {
            if (board[i][j])
            {
                ChessPiece* piece = board[i][j];
                if(piece->isWhite() == isWhite)
                {
                    pieces.push_back(piece);
                }
            }
        }
    }
    return pieces;
}

ChessPiece *ChessBoard::getKing(bool isWhite)
{
    if(isWhite)
    {
        return whiteKing;
    }
    else
    {
        return blackKing;
    }
}
