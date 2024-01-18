#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include "chesspiece.h"
#include <QObject>
#include <position.h>

class ChessMove : public QObject
{
    Q_OBJECT
public:
    explicit ChessMove(ChessPiece* movedPiece, Position originalPosition, Position newPosition, QObject *parent = nullptr);

    ChessPiece *getMovedPiece() const;
    Position getOriginalPosition() const;
    Position getNewPosition() const;

private:
    ChessPiece* movedPiece;
    Position originalPosition;
    Position newPosition;

signals:

};

#endif // CHESSMOVE_H
