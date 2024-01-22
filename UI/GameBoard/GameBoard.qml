import QtQuick
import QtQuick.Dialogs
import QtQuick.Controls 2.15
import com.chess 1.0
Item
{
     property var validMoves: []
    Repeater
    {
        model: 64

        Rectangle
        {
             property int row: Math.floor(index/8)
             property int col: index % 8
             property int size: 100

             x: col * size
             y: (7 - row) * size
             height: size
             width: size

             color:
             {
                 var even = ((row + col) % 2) == 0;
                 return even ? '#769656' : '#eeeed2' // to-do different colors
             }
         }
    }

    GameOverScreen {
        id: gameOverScreen
    }

    Repeater
    {
        id: piecerepeater
        model: GameController.pieceGeneration();

        delegate: Loader
        {
            active: modelData.name !== " " && !modelData.captured

            sourceComponent: Image
            {
                source: "../Assets/" + modelData.name + ".png";
                x: (modelData.column - 1) * 100
                y: (8 - modelData.row) * 100
                height: 100
                width: 100

                Rectangle
                {
                    visible: (modelData.name.includes("King") &&
                              ((modelData.isWhite() && GameController.whiteKingInCheck) ||
                               (!modelData.isWhite() && GameController.blackKingInCheck)))
                    color: "red"
                    opacity: 0.5
                    anchors.fill: parent
                }


                MouseArea
                {
                    id: dragArea

                    anchors.fill: parent
                    drag.target: GameController.isGameOver ? null : parent
                    drag.axis: Drag.XAndYAxis
                    drag.filterChildren: true

                    property real originalX: 0
                    property real originalY: 0

                    onPressed:
                    {
                        if (GameController.isGameOver)
                        {
                            drag.target = null;
                            return;
                        }

                        originalX = parent.x
                        originalY = parent.y;
                        if ((modelData.isWhite() && GameController.isWhiteTurn) || (!modelData.isWhite() && !GameController.isWhiteTurn))
                        {
                           validMoves = GameController.getValidMoves(modelData);
                        }
                    }

                    onReleased:
                    {
                        if (GameController.isGameOver)
                        {
                            return;
                        }

                        var dropColumn = Math.round(parent.x / 100) + 1;
                        var dropRow = 8 - Math.round(parent.y / 100);

                        console.log("DropColumn", dropColumn, "DropRow", dropRow);

                        if (((modelData.isWhite() && GameController.isWhiteTurn) || (!modelData.isWhite() && !GameController.isWhiteTurn)))
                        {
                            if(GameController.isValidMove(modelData, dropRow, dropColumn))
                            {
                                parent.x = (dropColumn - 1) * 100;
                                parent.y = (8 - dropRow) * 100;
                            }
                            else
                            {
                                parent.x = originalX;
                                parent.y = originalY;
                            }
                        }
                        else
                        {
                            parent.x = originalX;
                            parent.y = originalY;
                        }
                        validMoves = [];

                        if (!GameController.isPlayerTurn() && GameController.GameMode == "computer" && !GameController.isGameOver) {
                            GameController.completeAIMove();
                        }
                    }

                    Connections
                    {
                        target: GameController

                        function onPieceCaptured(capturedPiece, capturedRow, capturedColumn)
                        {
                             if (modelData.row === capturedRow && modelData.column === capturedColumn) {
                                modelData.captured = true;
                            }
                        }
                        function onWhiteKingInCheckChanged(king) {
                            console.log("White King In Check Changed:", GameController.whiteKingInCheck);
                        }

                        function onBlackKingInCheckChanged(king) {
                            console.log("Black King In Check Changed:", GameController.blackKingInCheck);
                        }

                        function onPawnPromotion(piece, row, column)
                        {
                            promotionDialog.pieceToPromote = piece
                            promotionDialog.visible = true
                        }

                        function onGameOver(isWin, isDraw) {
                            gameOverScreen.visible = true;
                            if (isWin) {
                                gameOverScreen.setMessage("Congratulations! You won!");
                            } else if (isDraw) {
                                gameOverScreen.setMessage("It's a draw!");
                            } else {
                                gameOverScreen.setMessage("You lost. Better luck next time!");
                            }
                        }

                        function onBoardReset() {
                            piecerepeater.model = GameController.pieceGeneration();
                        }
                    }
                }

                Component.onCompleted:
                {
                    //console.log("Created image with source:", source, "at position:", x, y, "Column Data: ", modelData.column);
                }
            }

        }
    }

    Repeater {
        model: validMoves

        delegate: Rectangle {
            x: (modelData.column - 1) * 100
            y: (8 - modelData.row) * 100
            width: 100
            height: 100
            color: "transparent"

            Rectangle
            {
                anchors.centerIn: parent
                width: 40
                height: 40
                radius: 20
                color: "blue"
                opacity: 0.5
            }
        }
    }

    Text {
        x: 800
        y: 10
        text: "Move Counter: " + GameController.moveCounter
        font.pixelSize: 20
        color: "black"
    }

    Item
    {
        focus: true
        //Keys.onLeftPressed: console.log(GameController.FEN)
    }

    Dialog {
        id: promotionDialog
        title: "Pawn Promotion"
        visible: false

        property ChessPiece pieceToPromote: null

        Column {
            Button {
                text: "Queen"
                onClicked: {
                    GameController.completePromotion(promotionDialog.pieceToPromote, "Queen")
                    promotionDialog.close()
                }
            }
            Button {
                text: "Knight"
                onClicked: {
                    GameController.completePromotion(promotionDialog.pieceToPromote, "Knight")
                    promotionDialog.close()
                }
            }
            Button {
                text: "Rook"
                onClicked: {
                    GameController.completePromotion(promotionDialog.pieceToPromote, "Rook")
                    promotionDialog.close()
                }
            }
            Button {
                text: "Bishop"
                onClicked: {
                    GameController.completePromotion(promotionDialog.pieceToPromote, "Bishop")
                    promotionDialog.close()
                }
            }
        }

    }
}


