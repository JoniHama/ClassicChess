#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QObject>
#include <position.h>
#include <iostream>
#include <QtQml>

class ChessPiece : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool captured READ isCaptured WRITE setCaptured NOTIFY capturedChanged)
    Q_PROPERTY(int row READ row NOTIFY positionChanged)
    Q_PROPERTY(int column READ column NOTIFY positionChanged)
public:
    explicit ChessPiece(int id, QString name, QString type = QString(), Position position = Position(-1, -1), QObject *parent = nullptr);

    QString name() const;

    QString type() const;

    void print() const;

    int id() const;

    int row() const {return m_position.row;}

    int column() const {return m_position.column;}

    //void update_location(int x, int y);

    Q_INVOKABLE bool isWhite() const;

    Position position() const;

    void setPosition(const Position& position);

    bool isCaptured() const;

    bool canCapture(const Position& targetPos) const;

    void setCaptured(bool newCaptured);

    void changeType(QString type);

signals:

    void nameChanged();

    void positionChanged();

    void capturedChanged();

private:
    QString m_name;
    QString m_type;
    int m_row;
    int m_id;
    int m_col;
    int m_column;
    Position m_position;
    bool m_captured = false;
};

#endif // CHESSPIECE_H
