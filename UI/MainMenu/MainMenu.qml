import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.chess 1.0

Image
{
    id: menuBackground
    width: 800; height: 600
    source: "../Assets/background.jpg"

    RowLayout
    {
        anchors
        {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            leftMargin: 50
            topMargin: 100
        }
        spacing: 50
        Layout.leftMargin: 50

        Button {
            id: singleButton

            text: "Against the computer"
            onClicked:
            {
                // to-do, check validity
                if(fen_box.text == "")
                {
                    GameController.FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
                }
                else
                {
                    GameController.FEN = fen_box.text;
                }
                GameController.GameMode = "computer";
                menuLoader.source = "GameBoard/GameBoard.qml"
            }
        }

        Button {
             id: multiButton

             text: "Local multiplayer"
             onClicked:
             {
                 // to-do, check validity
                 if(fen_box.text == "")
                 {
                     GameController.FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
                 }
                 else
                 {
                     GameController.FEN = fen_box.text;
                 }
                 GameController.GameMode = "local";
                 menuLoader.source = "GameBoard/GameBoard.qml"
             }

         }
         Button {
             id: settingsButton

             text: "Unused"
             onClicked: model.revert()
         }
         Button {
             id: quitButton

             text: "Quit"
             onClicked: Qt.quit()
         }

         TextField
         {
             id: fen_box
             placeholderText: qsTr("Enter FEN; default if empty")
         }

    }
}
