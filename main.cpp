#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "gamecontroller.h"

static QObject *gameControllerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    GameController *controller = new GameController();
    return controller;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    app.setWindowIcon(QIcon("ChessWithFriends/UI/Assets/icon.png"));

    qmlRegisterSingletonType<GameController>("com.chess", 1, 0, "GameController", gameControllerProvider);
    qmlRegisterType<ChessPiece>("com.chess", 1, 0, "ChessPiece");
    qmlRegisterType<Position>("com.chess", 1, 0, "Position");

    const QUrl url(u"qrc:/ChessWithFriends/UI/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
