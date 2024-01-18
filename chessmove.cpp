#include "chessmove.h"

ChessMove::ChessMove(ChessPiece* movedPiece, Position originalPosition, Position newPosition, QObject *parent)
    : QObject(parent), movedPiece(movedPiece), originalPosition(originalPosition), newPosition(newPosition)
{
}

ChessPiece *ChessMove::getMovedPiece() const
{
    if(movedPiece != nullptr)
    {
        return movedPiece;
    }
    else
    {
        return nullptr;
    }
}

Position ChessMove::getOriginalPosition() const
{
    return originalPosition;
}

Position ChessMove::getNewPosition() const
{
    return newPosition;
}
