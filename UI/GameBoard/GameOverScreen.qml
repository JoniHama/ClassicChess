import QtQuick 2.0
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import com.chess 1.0

Rectangle {
    id: gameOverOverlay
    anchors.fill: parent
    color: "#00000099"
    visible: false

    function setMessage(newText) {
        endMessage.text = newText;
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
        }
    }

    Rectangle {
        id: dialogBox
        width: parent.width * 0.5
        height: parent.height * 0.3
        color: "#2a2a2a"
        radius: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -parent.width * 0.15

        layer.enabled: true
        layer.effect: DropShadow {
            verticalOffset: 3
            horizontalOffset: 3
            color: "#80000000"
            radius: 8
        }

        Column {
            anchors {
                fill: parent
                margins: 20
            }
            spacing: 10

            Text {
                id: endMessage
                text: "Game Over"
                font.pixelSize: dialogBox.width * 0.1
                color: "#fff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
            }

            Button {
                text: "Restart Game"
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    GameController.restartGame();
                    gameOverOverlay.visible = false
                }
            }

            Button {
                text: "Return to main menu"
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                   GameController.restartGame();
                   gameOverOverlay.visible = false
                   menuLoader.source = "UI/MainMenu/MainMenu.qml"
                }
            }
        }
    }
}
