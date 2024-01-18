#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <chesspiece.h>
#include <chessmove.h>
#include <QObject>
#include <QDebug>
#include <QTextStream>
#include <QChar>
#include <array>
#include <memory>

class ChessBoard : public QObject
{
    Q_OBJECT
public:
    explicit ChessBoard(QObject *parent = nullptr);
    ~ChessBoard();

    ChessPiece* getPiece(const Position& position) const;

    void setPiece(const Position& position, ChessPiece* piece);

    void print();

    bool pieceAlreadyAt(const Position& position);

    std::vector<ChessPiece*> getPlayerPieces(bool isWhite);

    ChessPiece *getKing(bool isWhite);

    ChessMove* last_move = nullptr;

    ChessPiece* whiteKing = nullptr;
    ChessPiece* blackKing = nullptr;

private:
    std::array<std::array<ChessPiece*, 9>, 9> board;

signals:

};

#endif // CHESSBOARD_H
