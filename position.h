#ifndef POSITION_H
#define POSITION_H

#include <QDebug>

struct Position {
    int row;
    int column;

    Position() : row(-1), column(-1) {}
    Position(int row, int column) : row(row), column(column) {}

    bool operator==(const Position& other) const
    {
        return row == other.row && column == other.column;
    }

    bool operator!=(const Position& other) const
    {
        return !(*this == other);
    }
};

inline QDebug operator<<(QDebug debug, const Position &position)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << position.row << ", " << position.column << ')';
    return debug;
}

#endif // POSITION_H
