#include "chesspiece.h"

ChessPiece::ChessPiece(int id, QString name, QString type, Position position, QObject *parent)
    : QObject(parent), m_id(id), m_name(name), m_type(type), m_position(position) {
}
QString ChessPiece::name() const
{
    return m_name;
}

QString ChessPiece::type() const
{
    return m_type;
}

void ChessPiece::print() const
{
    std::cout << "Name: " << type().toStdString() << std::endl;
    std::cout << "Position: (" << m_position.row << ", " << m_position.column << ")" << std::endl;
    std::cout << std::endl;
}

int ChessPiece::id() const
{
    return m_id;
}

bool ChessPiece::isWhite() const
{
    return name().startsWith('W');
}

Position ChessPiece::position() const
{
    return m_position;
}

void ChessPiece::setPosition(const Position& position)
{
    if (m_position.row == position.row && m_position.column == position.column)
        return;
    m_position = position;
    emit positionChanged();
}

bool ChessPiece::isCaptured() const
{
    return m_captured;
}

bool ChessPiece::canCapture(const Position &targetPos) const
{
    return false;
}

void ChessPiece::setCaptured(bool newCaptured)
{
    if (m_captured == newCaptured)
        return;
    m_captured = newCaptured;
    emit capturedChanged();
}

void ChessPiece::changeType(QString type)
{
    if (m_name.contains("_Pawn"))
    {
        QStringList parts = m_name.split("_");
        if (parts.size() == 2) {
            m_name = parts[0] + "_" + type;
            m_type = type;
            emit nameChanged();
        }
    }
}
