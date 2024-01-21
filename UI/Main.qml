import QtQuick
import QtQuick.Window

Window {
    width: 1200
    height: 800
    visible: true
    title: qsTr("ClassicChess")

    Loader
    {
        id: menuLoader
        anchors.fill: parent
        source: "./MainMenu/MainMenu.qml"
    }
}
