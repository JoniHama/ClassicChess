#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "chessboard.h"
#include <QObject>
#include <QtQml>
#include <string>
#include <iostream>
#include <chesspiece.h>
#include <random>
#include <chrono>
#include <position.h>
#include <memory>

class GameController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool gameStarted READ gameStarted WRITE setgameStarted NOTIFY gameStartedChanged)
    Q_PROPERTY(QString FEN READ FEN WRITE setFEN NOTIFY FENChanged)
    Q_PROPERTY(QString GameMode READ GameMode WRITE setGameMode NOTIFY GameModeChanged)
    Q_PROPERTY(bool isWhiteTurn READ isWhiteTurn WRITE setIsWhiteTurn NOTIFY isWhiteTurnChanged)
    Q_PROPERTY(int moveCounter READ moveCounter WRITE setCounter NOTIFY moveCounterChanged)
    Q_PROPERTY(bool whiteKingInCheck READ whiteKingInCheck NOTIFY whiteKingInCheckChanged)
    Q_PROPERTY(bool blackKingInCheck READ blackKingInCheck NOTIFY blackKingInCheckChanged)
    Q_PROPERTY(bool isGameOver READ isGameOver WRITE setGameOver NOTIFY gameOverChanged)
    QML_ELEMENT
public:
    explicit GameController(QObject *parent = nullptr);

    struct GameState
    {
        bool isWhiteInCheck = false;
        bool isBlackInCheck = false;
    };

    bool gameStarted() const;

    bool isGameOver() const { return m_gameOver; }
    void setGameOver(bool gameOver)
    {
        if (m_gameOver == gameOver)
            return;

        m_gameOver = gameOver;
        emit gameOverChanged();
    }

    void setCounter(int number);

    bool isPieceOwn(ChessPiece* piece, Position newPosition);

    bool isPawnMoveValid(ChessPiece* piece, Position newPosition, bool checkMoves = true);

    bool isKnightMoveValid(ChessPiece* piece, Position newPosition);

    bool isBishopMoveValid(ChessPiece* piece, Position newPosition);

    bool isRookMoveValid(ChessPiece* piece, Position newPosition);

    bool isQueenMoveValid(ChessPiece* piece, Position newPosition);

    bool isKingMoveValid(ChessPiece* piece, Position newPosition);

    bool isPositionSafe(const Position& position, bool isWhite, bool excludeEnemyKing = false);

    bool isKingInCheck(bool isWhite);

    bool isKingCheckedAfterMove(ChessPiece* piece, Position newPosition);

    std::vector<std::pair<int, QVariantList> > RemoveCheckMoves(bool isWhite);

    GameState gameState;

    int depth = 1;

    std::vector<Position> validCastlingPositions = {};

    void completeCastling(ChessPiece* king, Position newPosition);

    void setForNextRound();

    void finishMove(ChessPiece* piece, Position oldPosition, Position newPosition);

    bool isEnPassant(ChessPiece* piece, Position currentPosition, Position newPosition, bool checkMoves);

    Q_INVOKABLE bool isValidMove(QObject* object, int row, int column);

    Q_INVOKABLE QList<QObject*> pieceGeneration();

    void DiagonalMoves(QList<Position>& validMoves, ChessPiece* piece, Position& currentPos, bool kingCheck = true);

    void StraightMoves(QList<Position>& validMoves, ChessPiece* piece, Position& currentPosm, bool kingCheck = true);

    QVariantList KingMoves(ChessPiece* piece);

    bool isCastlingValid(ChessPiece* king, ChessPiece* rook, Position newPosition);

    QList<Position> CastlingMoves(ChessPiece* piece);

    bool isKingNear(const Position& position1, const Position& position2);

    QVariantList getPawnAttacks(ChessPiece* piece);

    QList<Position> checkValidMoves(ChessPiece* piece, bool checkKing = true);

    Q_INVOKABLE QVariantList getValidMoves(ChessPiece* piece);

    ChessMove *generateRandomMove();

    Q_INVOKABLE void completeAIMove();

    void setgameStarted(bool newGameStarted);

    QString FEN() const;

    void setFEN(const QString &newFEN);

    Q_INVOKABLE void incrementMoveCounter();

    QString columnToChar(int column);

    QString moveText(Position whitePosition, Position blackPosition);

    Q_INVOKABLE bool isPlayerTurn();

    bool isWhiteTurn() const;

    void setIsWhiteTurn(bool newIsWhiteTurn);

    int moveCounter() const;

    bool whiteKingInCheck() const { return gameState.isWhiteInCheck; }
    bool blackKingInCheck() const { return gameState.isBlackInCheck; }

    QString GameMode() const;
    void setGameMode(const QString &newGameMode);

signals:
    void gameStartedChanged();
    void FENChanged();
    void pieceCaptured(ChessPiece* capturedPiece, int row, int column);
    void pawnPromotion(ChessPiece* piece, int row, int column);

    void whiteKingInCheckChanged();
    void blackKingInCheckChanged();

    void gameOver(bool isWin, bool isDraw);

    void isWhiteTurnChanged();

    void moveCounterChanged();

    void gameOverChanged();

    void boardReset();

    void GameModeChanged();

public slots:
    void completePromotion(ChessPiece* piece, const QString& newType);

    void restartGame();

private:
    ChessBoard* current_board;
    bool m_gameStarted;
    QString m_FEN;
    bool m_isWhiteTurn = true;
    bool m_gameOver = false;
    int m_moveCounter = 0;
    QString m_GameMode;
};

#endif // GAMECONTROLLER_H
