#include "gamecontroller.h"

GameController::GameController(QObject *parent)
    : QObject{parent},
    m_gameStarted(true)
{

}

bool GameController::gameStarted() const
{
    return m_gameStarted;
}

void GameController::setCounter(int number)
{
    if (m_moveCounter == number)
        return;

    m_moveCounter = number;
    emit moveCounterChanged();
}

bool GameController::isPieceOwn(ChessPiece *piece, Position newPosition)
{
    auto pieceAtNewPosition = current_board->getPiece(newPosition);
    if(pieceAtNewPosition && piece->isWhite() == pieceAtNewPosition->isWhite()) {return true;}
    return false;
}

bool GameController::isEnPassant(ChessPiece* piece, Position currentPosition, Position newPosition, bool checkMoves)
{
    if(piece->type() != "Pawn") {return false;}

    int direction = piece->isWhite() ? 1 : -1;

    if(currentPosition.row + direction != newPosition.row)
    {
        return false;
    }

    auto lastmove = current_board->last_move;
    if(!lastmove) {return false;}

    ChessPiece* movedPiece = lastmove->getMovedPiece();
    if(!movedPiece) {return false;}

    if(lastmove->getMovedPiece()->type() != "Pawn" || ((lastmove->getOriginalPosition().row - lastmove->getNewPosition().row != direction*2)))
    {
        return false;
    }

    bool isAdjacent = std::abs(currentPosition.column - lastmove->getOriginalPosition().column) == 1;
    bool correctRow = newPosition.row == lastmove->getOriginalPosition().row - direction;
    bool correctColumn = newPosition.column == lastmove->getNewPosition().column;

    if (isAdjacent && correctRow && correctColumn)
    {
        if(!checkMoves)
        {
            emit pieceCaptured(lastmove->getMovedPiece(), lastmove->getNewPosition().row, lastmove->getNewPosition().column);
            current_board->setPiece(lastmove->getNewPosition(), nullptr);
        }
        return true;
    }

    return false;
}

bool GameController::isPawnMoveValid(ChessPiece* piece, Position newPosition, bool checkMoves)
{
    Position currentPosition(piece->position());
    auto pieceAtNewPosition = current_board->getPiece(newPosition);

    if(piece->isWhite())
    {
        if(currentPosition.column == newPosition.column)
        {
            if((newPosition.row - currentPosition.row) == 2 && currentPosition.row == 2)
            {
                return !current_board->pieceAlreadyAt(newPosition) && !current_board->pieceAlreadyAt(Position(newPosition.row-1, newPosition.column));
            }
            else if((newPosition.row - currentPosition.row) == 1)
            {
                return !current_board->pieceAlreadyAt(newPosition);
            }
        }
        else if (std::abs(currentPosition.column - newPosition.column) == 1 && (newPosition.row - currentPosition.row == 1) && pieceAtNewPosition && (pieceAtNewPosition->isWhite() != piece->isWhite()))
        {
            return current_board->pieceAlreadyAt(newPosition);
        }
        else if(isEnPassant(piece, currentPosition, newPosition, checkMoves))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(currentPosition.column == newPosition.column)
        {
            if((newPosition.row - currentPosition.row) == -2 && currentPosition.row == 7)
            {
                return !current_board->pieceAlreadyAt(newPosition) && !current_board->pieceAlreadyAt(Position(newPosition.row+1, newPosition.column));
            }
            else if((newPosition.row - currentPosition.row) == -1)
            {
                return !current_board->pieceAlreadyAt(newPosition);
            }
        }
        else if (std::abs(currentPosition.column - newPosition.column) == 1 && (newPosition.row - currentPosition.row == -1) && pieceAtNewPosition && (pieceAtNewPosition->isWhite() != piece->isWhite()))
        {
            return current_board->pieceAlreadyAt(newPosition);
        }
        else if(isEnPassant(piece, currentPosition, newPosition, checkMoves))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool GameController::isKnightMoveValid(ChessPiece *piece, Position newPosition)
{
    Position currentPosition(piece->position());

    if(isPieceOwn(piece, newPosition)){return false;}

    int rowDiff = abs(newPosition.row - currentPosition.row);
    int colDiff = abs(newPosition.column - currentPosition.column);

    if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))
    {
        if (newPosition.row >= 1 && newPosition.row <= 8 && newPosition.column >= 1 && newPosition.column <= 8)
        {
            return true;
        }
    }
    return false;
}

bool GameController::isBishopMoveValid(ChessPiece *piece, Position newPosition)
{
    int newRow = newPosition.row;
    int newColumn = newPosition.column;

    int currentRow = piece->row();
    int currentColumn = piece->column();

    int rowDirection = (newRow > currentRow) ? 1 : -1;
    int columnDirection = (newColumn > currentColumn) ? 1 : -1;

    if(isPieceOwn(piece, newPosition))
    {
        //qDebug() << "1. bishopmovevalid " << newPosition << " tiellä oli " << current_board->getPiece(newPosition)->name();
        return false;
    }

    if (abs(newRow - currentRow) != abs(newColumn - currentColumn))
    {
                //qDebug() << "2. bishopmovevalid " << newPosition;
        return false;
    }

    int checkRow = currentRow + rowDirection;
    int checkColumn = currentColumn + columnDirection;
    while (checkRow != newRow && checkColumn != newColumn)
    {
        if (current_board->getPiece(Position{checkRow, checkColumn}) != nullptr)
        {
                            //qDebug() << "3. bishopmovevalid " << newPosition;
            return false;
        }
        checkRow += rowDirection;
        checkColumn += columnDirection;
    }

    return true;
}

bool GameController::isRookMoveValid(ChessPiece *piece, Position newPosition)
{
    int newRow = newPosition.row;
    int newColumn = newPosition.column;

    int currentRow = piece->row();
    int currentColumn = piece->column();

    if(currentRow != newRow && currentColumn != newColumn) {return false;}

    if(isPieceOwn(piece, newPosition)){return false;}

    int rowDirection = (newRow == currentRow) ? 0 : (newRow > currentRow ? 1 : -1);
    int columnDirection = (newColumn == currentColumn) ? 0 : (newColumn > currentColumn ? 1 : -1);

    int checkRow = currentRow + rowDirection;
    int checkColumn = currentColumn + columnDirection;

    while (checkRow != newRow || checkColumn != newColumn)
    {
        if (current_board->getPiece(Position{checkRow, checkColumn}) != nullptr)
        {
            return false;
        }
        checkRow += rowDirection;
        checkColumn += columnDirection;
    }

    return true;
}

bool GameController::isQueenMoveValid(ChessPiece *piece, Position newPosition)
{
    int newRow = newPosition.row;
    int newColumn = newPosition.column;

    int currentRow = piece->row();
    int currentColumn = piece->column();

    bool isStraight = (currentRow == newRow || currentColumn == newColumn);
    bool isDiagonal = (abs(newRow - currentRow) == abs(newColumn - currentColumn));

    if(!isStraight && !isDiagonal)
    {
        return false;
    }

    if(isPieceOwn(piece, newPosition)) {return false;}

    int rowDirection = (newRow == currentRow) ? 0 : (newRow > currentRow ? 1 : -1);
    int columnDirection = (newColumn == currentColumn) ? 0 : (newColumn > currentColumn ? 1 : -1);

    int checkRow = currentRow + rowDirection;
    int checkColumn = currentColumn + columnDirection;

    while (checkRow != newRow || checkColumn != newColumn)
    {
        if (current_board->getPiece(Position{checkRow, checkColumn}) != nullptr)
        {
            return false;
        }
        checkRow += rowDirection;
        checkColumn += columnDirection;
    }

    return true;
}

bool GameController::isKingMoveValid(ChessPiece* piece, Position newPosition)
{
    int rowDiff = abs(newPosition.row - piece->row());
    int columnDiff = abs(newPosition.column - piece->column());

    if(isPieceOwn(piece, newPosition)){return false;}

    if(rowDiff > 1 || columnDiff > 1)
    {
        return false;
    }

    if(newPosition.row > 8 || newPosition.column > 8) {return false;}

    if (!isPositionSafe(newPosition, piece->isWhite(), true))
    {
        return false;
    }

    return true;
}

bool GameController::isPositionSafe(const Position &position, bool isWhite, bool excludeEnemyKing)
{
    //qDebug() << "Checking position safety for:" << position.column << position.row;
    for (const auto& piece : current_board->getPlayerPieces(!isWhite))
    {
        if (excludeEnemyKing && piece->type() == "King")
        {
            if (isKingNear(position, piece->position()))
            {
                return false;
            }
            continue;
        }
        QVariantList validMoves;
        if(piece->type() == "Pawn")
        {
            validMoves = getPawnAttacks(piece);
        }
        else
        {
            QList<Position> moves = checkValidMoves(piece, false);
            for (const auto& movePosition : moves)
            {
                //if(piece->type() == "Queen"){qDebug() << movePosition.column << "ja" << movePosition.row;}
                QVariantMap moveMap;
                moveMap["row"] = movePosition.row;
                moveMap["column"] = movePosition.column;
                validMoves.append(moveMap);
            }
        }

        for (const auto& moveVariant : validMoves)
        {
            QVariantMap moveMap = moveVariant.toMap();
            Position movePosition(moveMap["row"].toInt(), moveMap["column"].toInt());
            if (movePosition == position)
            {
                //qDebug() << "Vaarallinen positio!" << position.column << position.row << " " << piece->name() << " toimesta!";
                return false;
            }
            else
            {
                //qDebug() << "Turvallinen positio!" << position.column << position.row << " " << piece->name() << " johon tämä ei pääse!" << " koska se voi liikkua vain tässä tapauksessa " << movePosition.column << position.row;
            }
        }
    }
    return true;
}

bool GameController::isKingInCheck(bool isWhite)
{
    ChessPiece* king = current_board->getKing(isWhite);

    if(!king) {return false;}

    return !isPositionSafe(king->position(), isWhite);
}

bool GameController::isKingCheckedAfterMove(ChessPiece *piece, Position newPosition)
{
    Position currentPosition = piece->position();
    ChessPiece* pieceAtNewPosition = current_board->getPiece(newPosition);
    bool moveValid = false;

    current_board->setPiece(currentPosition, nullptr);
    current_board->setPiece(newPosition, piece);
    piece->setPosition(newPosition);

    bool kingInCheck = isKingInCheck(piece->isWhite());

    current_board->setPiece(newPosition, pieceAtNewPosition);
    current_board->setPiece(currentPosition, piece);
    piece->setPosition(currentPosition);

    return kingInCheck;
}

std::vector<std::pair<int, QVariantList>> GameController::RemoveCheckMoves(bool isWhite)
{
    std::vector<std::pair<int, QVariantList>> validMoves;
    std::vector<ChessPiece*> playerPieces = current_board->getPlayerPieces(isWhite);

    for (auto& piece : playerPieces)
    {
        QVariantList pieceMoves;
        QList<Position> moves = checkValidMoves(piece);
        for (const auto& movePosition : moves)
        {
            Position originalPosition = piece->position();
            piece->setPosition(movePosition);
            current_board->setPiece(originalPosition, nullptr);
            auto oldobject = current_board->getPiece(movePosition);
            current_board->setPiece(movePosition, piece);

            bool checkResolved = !isKingInCheck(isWhite);

            piece->setPosition(originalPosition);
            current_board->setPiece(movePosition, oldobject);
            current_board->setPiece(originalPosition, piece);

            if (checkResolved)
            {
                QVariantMap moveVariant;
                moveVariant["row"] = movePosition.row;
                moveVariant["column"] = movePosition.column;
                pieceMoves.append(moveVariant);
            }
        }
        if (!pieceMoves.isEmpty())
        {
            validMoves.push_back(std::make_pair(piece->id(), pieceMoves));
        }
    }
    return validMoves;
}

void GameController::completeCastling(ChessPiece *king, Position newPosition)
{
    Position kingPosition = king->position();
    Position rookPosition = {};

    int direction = (newPosition.column > kingPosition.column) ? 1 : -1;

    Position possibleRookPosition = {kingPosition.row, kingPosition.column + 3*direction};
    Position possibleRookPosition2 = {kingPosition.row, kingPosition.column + 4*direction};

    ChessPiece* possibleRook = current_board->getPiece(possibleRookPosition);
    ChessPiece* possibleRook2 = current_board->getPiece(possibleRookPosition2);

    if(possibleRook && possibleRook->type() == "Rook")
    {
        rookPosition = possibleRookPosition;
    }
    else if(possibleRook2 && possibleRook2->type() == "Rook")
    {
        rookPosition = possibleRookPosition2;
    }

    ChessPiece* rook = current_board->getPiece(rookPosition);

    Position newKingPosition = newPosition;
    Position newRookPosition = (direction == 1) ? Position{kingPosition.row, newKingPosition.column - 1}
                                                : Position{kingPosition.row, newKingPosition.column + 1};

    //qDebug() << "New rook position" << newRookPosition;

    current_board->setPiece(kingPosition, nullptr);
    current_board->setPiece(newKingPosition, king);
    king->setPosition(newKingPosition);

    current_board->setPiece(rookPosition, nullptr);
    current_board->setPiece(newRookPosition, rook);
    rook->setPosition(newRookPosition);
}

void GameController::setForNextRound()
{
    setIsWhiteTurn(!isWhiteTurn());
    incrementMoveCounter();
    validCastlingPositions = {};
}

void GameController::finishMove(ChessPiece *piece, Position oldPosition, Position newPosition)
{
    if(current_board->pieceAlreadyAt(newPosition))
    {
        ChessPiece* capturedPiece = current_board->getPiece(newPosition);
        emit pieceCaptured(capturedPiece, newPosition.row, newPosition.column);
    }
    if(gameState.isBlackInCheck == true)
    {
        //qDebug() << "Black no longer in check!";
        gameState.isBlackInCheck = false;
        emit blackKingInCheckChanged();
    }
    else if(gameState.isWhiteInCheck == true)
    {
        //qDebug() << "White no longer in check!";
        gameState.isWhiteInCheck = false;
        emit whiteKingInCheckChanged();
    }

    current_board->setPiece(oldPosition, nullptr);
    current_board->setPiece(newPosition, piece);
    piece->setPosition(newPosition);
    setForNextRound();
    current_board->print();
    ChessMove* latestMove = new ChessMove(piece, oldPosition, newPosition);
    current_board->last_move = latestMove;

    if(isKingInCheck(true))
    {
        //qDebug() << "White in check!";
        gameState.isWhiteInCheck = true;
        emit whiteKingInCheckChanged(current_board->whiteKing);
    }
    if(isKingInCheck(false))
    {
        //qDebug() << "Black in check!";
        gameState.isBlackInCheck = true;
        emit blackKingInCheckChanged(current_board->blackKing);
    }

    // check for stalemate
    auto possibleMoves = RemoveCheckMoves(isWhiteTurn());

    if(possibleMoves.empty() && isKingInCheck(isWhiteTurn()))
    {
        setGameOver(true);
        emit gameOver(true, false);
    }
    else if(possibleMoves.empty())
    {
        setGameOver(true);
        emit gameOver(false, true);
    }

}

void GameController::setgameStarted(bool newGameStarted)
{
    if (m_gameStarted == newGameStarted)
        return;
    m_gameStarted = newGameStarted;
    emit gameStartedChanged();
}

bool GameController::isValidMove(QObject* object, int row, int column)
{
    ChessPiece* piece = qobject_cast<ChessPiece*>(object);

    Position oldPosition(piece->position());
    Position newPosition{row, column};

    if(oldPosition == newPosition || isKingCheckedAfterMove(piece, newPosition)) {return false;}


    if(gameState.isWhiteInCheck && piece->isWhite() || gameState.isBlackInCheck && !piece->isWhite())
    {
        auto possibleMoves = RemoveCheckMoves(isWhiteTurn());

        if(possibleMoves.empty())
        {
            //qDebug() << "Checkmate!";
        }
        else
        {
            for(auto& possiblemove : possibleMoves)
            {
                if(possiblemove.first == piece->id())
                {
                    finishMove(piece, oldPosition, newPosition);
                    return true;
                }
            }
            return false;
        }
    }


    if(piece)
    {
        if(piece->type() == "Pawn")
        {
            if(isPawnMoveValid(piece, newPosition, false))
            {
                bool isPromotion = (piece->type() == "Pawn") && ((newPosition.row == 8 && piece->isWhite())
                                                                 || (newPosition.row == 1 && !piece->isWhite()));
                if(isPromotion)
                {
                    emit pawnPromotion(piece, newPosition.row, newPosition.column);
                    return true;
                }
                finishMove(piece, oldPosition, newPosition);
                return true;
            }
            return false;
        }
        else if(piece->type() == "Knight")
        {
            if(isKnightMoveValid(piece, newPosition))
            {
                finishMove(piece, oldPosition, newPosition);
                return true;
            }
            return false;
        }
        else if (piece->type() == "Bishop")
        {
            if(isBishopMoveValid(piece, newPosition))
            {
                finishMove(piece, oldPosition, newPosition);
                return true;
            }
            return false;
        }
        else if(piece->type() == "Rook")
        {
            if(isRookMoveValid(piece, newPosition))
            {
                finishMove(piece, oldPosition, newPosition);
                return true;
            }
            return false;
        }
        else if(piece->type() == "Queen")
        {
            if(isQueenMoveValid(piece, newPosition))
            {
                finishMove(piece, oldPosition, newPosition);
                return true;
            }
            return false;
        }
        else if(piece->type() == "King")
        {
            if(isKingMoveValid(piece, newPosition))
            {
                finishMove(piece, oldPosition, newPosition);
                return true;
            }
            else
            {
                if(!validCastlingPositions.empty())
                {
                    for(auto position : validCastlingPositions)
                    {
                        if(position == newPosition) // to-do tornin päälle siirtäminenkin riittää
                        {
                            completeCastling(piece, position);
                            ChessMove* latestMove = new ChessMove(piece, oldPosition, newPosition);
                            current_board->last_move = latestMove;
                            setForNextRound();
                            return true;
                        }
                    }
                }
            }
            return false;
        }

    }
    else
    {
        qDebug() << "Error! Object is not a ChessPiece.";
    }

    return false;
}

QList<QObject*> GameController::pieceGeneration()
{
    QList<QObject*> pieces;
    ChessBoard* board = new ChessBoard();
    int pieceid = 0;

    current_board = board;

    QString type;

    int row = 8;
    int column = 1;

    QString piece_string;

    bool has_been_space = false;

    QString fen = FEN();

    for(auto chr : fen)
    {
        if(chr == 'w')
        {
            setIsWhiteTurn(true);
        }
        if(chr == '/')
        {
            row--;
            column = 1;
            continue;
        }

        if(column > 8)
        {
            column = 1;
        }

        piece_string.clear();

        if(chr == ' ')
        {
            has_been_space = true;
        }

        if(has_been_space)
        {
            //to-do
            break;
        }

        if(!chr.isDigit())
        {
            if(!chr.isUpper())
            {
                piece_string += "B";
            }
            else
            {
                piece_string += "W";
            }
            piece_string += "_";

            chr = chr.toUpper();

            if(chr == 'R')
            {
                piece_string += "Rook";
                type = "Rook";
            }
            else if (chr == 'P')
            {
                piece_string += "Pawn";
                type = "Pawn";
            }
            else if (chr == 'N')
            {
                piece_string += "Knight";
                type = "Knight";
            }
            else if (chr == 'B')
            {
                piece_string += "Bishop";
                type = "Bishop";
            }
            else if (chr == 'Q')
            {
                piece_string += "Queen";
                type = "Queen";
            }
            else if (chr == 'K')
            {
                piece_string += "King";
                type = "King";
            }
            ChessPiece* piece = new ChessPiece(pieceid, piece_string, type, Position{row, column}, this);

            if(piece->type() == "King")
            {
                if(piece->isWhite()) {current_board->whiteKing = piece;}
                else if(!piece->isWhite()) {current_board->blackKing = piece;}
            }

            board->setPiece(Position(row, column), piece);
            board->print();

            pieces.push_back(piece);
            pieceid++;
            column++;
        }
        else
        {
            for (int i = 0; i < chr.digitValue(); ++i)
            {
                ChessPiece* piece = new ChessPiece(-1, " ", " ", Position{row, column}, this);
                pieces.push_back(piece);
                column++;
            }
        }
    }

    return pieces;
}

void GameController::DiagonalMoves(QList<Position>& validMoves, ChessPiece *piece, Position& currentPos, bool kingCheck)
{
    const int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (const auto& dir : directions)
    {
        Position newPos = currentPos;
        while (true) {
            newPos.row += dir[0];
            newPos.column += dir[1];

            if (newPos.row < 1 || newPos.row > 8 || newPos.column < 1 || newPos.column > 8)
            {
                //qDebug() << "1. Break! " << newPos;
                break;
            }

            if (isBishopMoveValid(piece, newPos))
            {
                if(!kingCheck || !isKingCheckedAfterMove(piece, newPos))
                {
                    validMoves.append(newPos);
                }
            }

            if (current_board->getPiece(newPos) != nullptr)
            {
                //qDebug() << "4. Break! " << newPos;
                //qDebug() << newPos << "Tiellä oli " << current_board->getPiece(newPos)->name();
                break;
            }
        }
    }
}

void GameController::StraightMoves(QList<Position> &validMoves, ChessPiece *piece, Position &currentPos, bool kingCheck)
{
    for (int col = 1; col <= 8; ++col)
    {
        if (col == currentPos.column) continue;
        Position newPos(currentPos.row, col);
        if (isRookMoveValid(piece, newPos))
        {
            validMoves.append(newPos);
        }
    }

    for (int row = 1; row <= 8; ++row)
    {
        if (row == currentPos.row) continue;
        Position newPos(row, currentPos.column);
        if (isRookMoveValid(piece, newPos))
        {
            if(!kingCheck || !isKingCheckedAfterMove(piece, newPos))
            {
                validMoves.append(newPos);
            }
        }
    }
}

QVariantList GameController::KingMoves(ChessPiece *piece)
{
    QVariantList safeMoves;
    Position currentPos = piece->position();

    for (int row = -1; row <= 1; ++row)
    {
        for (int col = -1; col <= 1; ++col)
        {
            if (row == 0 && col == 0) continue;
            Position newPos(currentPos.row + row, currentPos.column + col);
            if (isKingMoveValid(piece, newPos))
            {
                QVariantMap moveMap;
                moveMap["row"] = newPos.row;
                moveMap["column"] = newPos.column;
                safeMoves.append(moveMap);
            }
        }
    }
    return safeMoves;
}

bool GameController::isCastlingValid(ChessPiece *king, ChessPiece *rook, Position newPosition)
{
    if(isKingInCheck(king->isWhite())) {return false;}

    if(king->isWhite())
    {
        if(current_board->whiteKing->position() != Position{1, 5}) {return false;}
    }
    else
    {
        if(current_board->blackKing->position() != Position{8, 5}) {return false;}
    }

    int direction = (newPosition.column > king->position().column) ? 1 : -1;
    Position currentPosition = king->position();

    for (int col = currentPosition.column + direction; col != rook->position().column; col += direction)
    {
        Position gapPosition(currentPosition.row, col);
        if (current_board->getPiece(gapPosition) || !isPositionSafe(gapPosition, king->isWhite()))
        {
            return false;
        }
    }

    if (!isPositionSafe(newPosition, king->isWhite())) return false;

    return true;
}

void GameController::checkForMate()
{
    auto possibleMoves = RemoveCheckMoves(isWhiteTurn());

    if(possibleMoves.empty())
    {
        qDebug() << "CHECKMATE!";
        setGameOver(true);
        emit gameOver(true, false);
    }
}

QList<Position> GameController::CastlingMoves(ChessPiece *piece)
{
    QList<Position> moves;

    bool isWhite = piece->isWhite();

    std::vector<ChessPiece*> playerpieces = current_board->getPlayerPieces(isWhite);
    std::vector<ChessPiece*> rooks;

    for(ChessPiece* playerpiece : playerpieces)
    {
        if(playerpiece->type() == "Rook")
        {
            rooks.push_back(playerpiece);
        }
    }


    for(ChessPiece* rook : rooks)
    {
        if(rook->isWhite())
        {
            if(!(rook->position() == Position{1, 1} || rook->position() == Position{1, 8}))
            {
                continue;
            }
            else
            {
                if(isCastlingValid(current_board->whiteKing, rook, Position{1, 7}))
                {
                    validCastlingPositions.push_back(Position{1, 7});
                    moves.append(Position{1, 7});
                }
                if(isCastlingValid(current_board->whiteKing, rook, Position{1, 2}))
                {
                    validCastlingPositions.push_back(Position{1, 3});
                    moves.append(Position{1, 3});
                }
            }
        }
        else
        {
            if(!(rook->position() == Position{8, 1} || rook->position() == Position{8, 8}))
            {
                continue;
            }
            else
            {
                if(isCastlingValid(current_board->blackKing, rook, Position{8, 7}))
                {
                    validCastlingPositions.push_back(Position{8, 7});
                    moves.append(Position{8, 7});
                }
                if(isCastlingValid(current_board->blackKing, rook, Position{8, 3}))
                {
                    validCastlingPositions.push_back(Position{8, 3});
                    moves.append(Position{8, 3});
                }
            }
        }
    }
    return moves;
}

bool GameController::isKingNear(const Position &position1, const Position &position2)
{
    int rowDiff = abs(position1.row - position2.row);
    int colDiff = abs(position1.column - position2.column);
    return rowDiff <= 1 && colDiff <= 1;
}

QVariantList GameController::getPawnAttacks(ChessPiece *piece)
{
    QList<Position> validMoves;
    QVariantList validMovesVariant;

    QString pieceType = piece->type();
    Position currentPos = piece->position();

    if(pieceType == "Pawn")
    {
        int direction = piece->isWhite() ? 1 : -1;

        Position captureLeft(currentPos.row + direction, currentPos.column - 1);
        Position captureRight(currentPos.row + direction, currentPos.column + 1);

        if (captureLeft.column >= 1 && captureLeft.column <= 8)
        {
            validMoves.append(captureLeft);
        }
        if (captureRight.column >= 1 && captureRight.column <= 8)
        {
            validMoves.append(captureRight);
        }
    }
    for (const Position& pos : validMoves)
    {
        QVariantMap posMap;
        posMap["row"] = pos.row;
        posMap["column"] = pos.column;
        validMovesVariant.append(posMap);
    }
    return validMovesVariant;
}

QList<Position> GameController::checkValidMoves(ChessPiece *piece, bool checkKing)
{
    QList<Position> validMoves;

    QString pieceType = piece->type();
    Position currentPos = piece->position();

    if(pieceType == "Pawn")
    {
        int direction = piece->isWhite() ? 1 : -1;
        int startRow = piece->isWhite() ? 2 : 7;

        Position oneStepForward(currentPos.row + direction, currentPos.column);
        if (isPawnMoveValid(piece, oneStepForward))
        {
            if(!checkKing || !isKingCheckedAfterMove(piece, oneStepForward))
            {
                validMoves.append(oneStepForward);
            }
        }

        if (currentPos.row == startRow)
        {
            Position twoStepsForward(currentPos.row + 2 * direction, currentPos.column);
            if (isPawnMoveValid(piece, twoStepsForward))
            {
                if(!checkKing || !isKingCheckedAfterMove(piece, twoStepsForward))
                {
                    validMoves.append(twoStepsForward);
                }
            }
        }

        Position captureLeft(currentPos.row + direction, currentPos.column - 1);
        Position captureRight(currentPos.row + direction, currentPos.column + 1);

        if (isPawnMoveValid(piece, captureLeft))
        {
            if(!checkKing || !isKingCheckedAfterMove(piece, captureLeft))
            {
                validMoves.append(captureLeft);
            }
        }
        if (isPawnMoveValid(piece, captureRight))
        {
            if(!checkKing || !isKingCheckedAfterMove(piece, captureRight))
            {
                validMoves.append(captureRight);
            }
        }

        //to-do en passant

    }
    else if(pieceType == "Knight")
    {
        const int rowMoves[8] = {-2, -2, -1, 1, 2, 2, 1, -1};
        const int columnMoves[8] = {-1, 1, 2, 2, 1, -1, -2, -2};
        for (int i = 0; i < 8; ++i)
        {
            Position newPos(currentPos.row + rowMoves[i], currentPos.column + columnMoves[i]);
            if (isKnightMoveValid(piece, newPos))
            {
                if(!checkKing || !isKingCheckedAfterMove(piece, newPos))
                {
                    validMoves.append(newPos);
                }
            }
        }
    }
    else if(pieceType == "Bishop")
    {
        DiagonalMoves(validMoves, piece, currentPos, checkKing);
    }
    else if(pieceType == "Rook")
    {
        StraightMoves(validMoves, piece, currentPos, checkKing);
    }
    else if(pieceType == "Queen")
    {
        DiagonalMoves(validMoves, piece, currentPos, checkKing);
        StraightMoves(validMoves, piece, currentPos, checkKing);
    }
    else if(pieceType == "King")
    {
        for (int row = -1; row <= 1; ++row)
        {
            for (int col = -1; col <= 1; ++col)
            {
                if (row == 0 && col == 0) continue;
                Position newPos(currentPos.row + row, currentPos.column + col);
                if (isKingMoveValid(piece, newPos))
                {
                    validMoves.append(newPos);
                }
            }
        }
    }
    return validMoves;
}

QVariantList GameController::getValidMoves(ChessPiece *piece)
{
    // optimize this by caching earlier moves if nothing has changed

    QList<Position> validMoves;
    QVariantList validMovesVariant;

    if((gameState.isWhiteInCheck && piece->isWhite()) || (gameState.isBlackInCheck && !piece->isWhite()))
    {
        auto possibleMoves = RemoveCheckMoves(isWhiteTurn());
        QVariantList validmoves;

        if(possibleMoves.empty())
        {
            qDebug() << "CHECKMATE!";
            setGameOver(true);
            emit gameOver(true, false);
        }
        else
        {
            for(auto& possiblemove : possibleMoves)
            {
                if(possiblemove.first == piece->id())
                {
                    validmoves.append(possiblemove.second);
                }
            }
        }
        return validmoves;
    }
    else
    {
    }

    validMoves = checkValidMoves(piece);

    if(piece->type() == "King")
    {
        validMoves.append(CastlingMoves(piece));
    }

    for (const Position& pos : validMoves)
    {
        QVariantMap posMap;
        posMap["row"] = pos.row;
        posMap["column"] = pos.column;
        validMovesVariant.append(posMap);
    }

    return validMovesVariant;
}

ChessMove* GameController::generateRandomMove()
{
    ChessPiece* randomPiece = nullptr;
    QVariantList moves;

    std::vector<ChessPiece*> pieces = current_board->getPlayerPieces(isPlayerTurn());

    if(pieces.empty()) {return nullptr;}

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::uniform_int_distribution<size_t> distribution(0, pieces.size() - 1);


    while(moves.isEmpty())
    {
        size_t randomIndex = distribution(generator);
        randomPiece = pieces[randomIndex];
        moves = getValidMoves(randomPiece);
    }

    if(!moves.isEmpty())
    {
        std::uniform_int_distribution<size_t> moveDistribution(0, moves.size() - 1);
        size_t randomMoveIndex = moveDistribution(generator);

        Position oldposition = randomPiece->position();
        QVariant selectedMoveVariant = moves[randomMoveIndex];
        QVariantMap selectedMoveMap = selectedMoveVariant.toMap();

        int row = selectedMoveMap["row"].toInt();
        int column = selectedMoveMap["column"].toInt();
        Position newposition = Position{row, column};

        return new ChessMove(randomPiece, oldposition, newposition, this);
    }

    return nullptr;

}

void GameController::completeAIMove()
{
    if (GameMode() != "computer" || isPlayerTurn())
    {
        return;
    }

    ChessMove* selectedMove = generateRandomMove();

    ChessPiece* movedPiece = selectedMove->getMovedPiece();

    if(movedPiece->type() == "Pawn" && (selectedMove->getNewPosition().row == 1 || selectedMove->getNewPosition().row == 8))
    {
        movedPiece->changeType("Queen");
    }

    //qDebug() << "AI move: " << "Piece: " << selectedMove->getMovedPiece()->name() << " Position change : " << selectedMove->getOriginalPosition() << selectedMove->getNewPosition();
    finishMove(selectedMove->getMovedPiece(), selectedMove->getOriginalPosition(), selectedMove->getNewPosition());
}

QString GameController::FEN() const
{
    return m_FEN;
}

void GameController::setFEN(const QString &newFEN)
{
    if (m_FEN == newFEN)
        return;
    m_FEN = newFEN;
    emit FENChanged();
}

void GameController::incrementMoveCounter()
{
    m_moveCounter++;
    emit moveCounterChanged();
}

QString GameController::columnToChar(int column)
{
    if (column < 1 || column > 8) {
        return "Invalid Column";
    }

    QString columns[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    return columns[column - 1];
}

QString GameController::moveText(Position whitePosition, Position blackPosition)
{
    QString startColumnLetter = columnToChar(whitePosition.column);
    QString endColumnLetter = columnToChar(blackPosition.column);

    return startColumnLetter + QString::number(whitePosition.row) + " " + endColumnLetter + QString::number(blackPosition.row);
}

bool GameController::isPlayerTurn()
{
    return isWhiteTurn();
}

bool GameController::isWhiteTurn() const
{
    return m_isWhiteTurn;
}

void GameController::setIsWhiteTurn(bool newIsWhiteTurn)
{
    if (m_isWhiteTurn == newIsWhiteTurn)
        return;
    m_isWhiteTurn = newIsWhiteTurn;
    emit isWhiteTurnChanged();
}

int GameController::moveCounter() const
{
    return m_moveCounter;
}

void GameController::completePromotion(ChessPiece *piece, const QString &newType, int newrow, int newcolumn)
{
    piece->changeType(newType);
    Position newPosition = Position{newrow, newcolumn};
    finishMove(piece, piece->position(), newPosition);
    if(GameMode() == "computer")
    {
        completeAIMove();
    }
}

void GameController::restartGame()
{
    delete current_board;
    setCounter(0);
    setIsWhiteTurn(true);
    gameState.isWhiteInCheck = false;
    gameState.isBlackInCheck = false;
    emit gameStartedChanged();
    setGameOver(false);
    emit boardReset();
}

QString GameController::GameMode() const
{
    return m_GameMode;
}

void GameController::setGameMode(const QString &newGameMode)
{
    if (m_GameMode == newGameMode)
        return;
    m_GameMode = newGameMode;
    emit GameModeChanged();
}
