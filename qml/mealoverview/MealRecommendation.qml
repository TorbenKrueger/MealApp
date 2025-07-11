import QtQuick 2.0
import QtQuick.Controls 2.0
import CModel 1.0

Rectangle {
    id: item1
    color: primaryColor
    Component.onCompleted: swipeTimer.start()
    Rectangle {
        id: rectangle
        height: 200
        color: primaryColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.rightMargin: 5
        anchors.leftMargin: 5

        ListModel {
            id: appModel
            ListElement { name: "Music"; icon: "pics/AudioPlayer_48.png" }
            ListElement { name: "Movies"; icon: "pics/VideoPlayer_48.png" }
            ListElement { name: "Camera"; icon: "pics/Camera_48.png" }
            ListElement { name: "Calendar"; icon: "pics/DateBook_48.png" }
            ListElement { name: "Messaging"; icon: "pics/EMail_48.png" }
            ListElement { name: "Todo List"; icon: "pics/TodoList_48.png" }
            ListElement { name: "Contacts"; icon: "pics/AddressBook_48.png" }
        }

        Component {
            id: appDelegate
            Rectangle {
                height: 100
                width: 100
                color:"red"
                border.color: "black"
            }
        }

        Component {
            id: appHighlight
            Rectangle { width: 80; height: 80; color: "lightsteelblue" }
        }

        PathView {
            id: view
            anchors.fill: parent
            highlight: appHighlight
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            focus: true
            model: appModel
            delegate: appDelegate
            path: Path {
                startX: 10
                startY: 50
                PathQuad { x: rectangle.width / 2; y: 50; controlX: x - 150; controlY: 50 }
                PathQuad { x: rectangle.width - 10; y: 50; controlX: x - 40; controlY: 50 }

                //                   PathQuad { x: rectangle.width / 2; y: 50; controlX: x - 150; controlY: 50 }
                //                   PathQuad { x: rectangle.width - 10; y: 50; controlX: x - 40; controlY: 50 }

                //                   PathQuad { x: 200; y: 150; controlX: 50; controlY: 200 }
                //                   PathQuad { x: 390; y: 50; controlX: 350; controlY: 200 }
            }
        }

        //        Component {
        //            id: delegate
        //            MealOverview {
        //                height: pathView.height
        //                width: pathView.width
        //                object: mealList.item(index)
        //                visible: true //index == pathView.currentIndex
        //                showToogleViewAction: true
        //            }
        //        }

        //        PathView {
        //                id: pathView
        //                anchors.fill: parent
        ////                snapMode: PathView.SnapOneItem
        ////                highlightRangeMode: PathView.StrictlyEnforceRange
        //                currentIndex: 1
        //                model: MealListModel {
        //                    id: model
        //                    list: mealList
        //                }
        //                delegate: delegate
        //                path: Path {
        //                    startX: -100//-pathView.width / 2  // let the first item in left
        //                    startY: pathView.height / 2  // item's vertical center is the same as line's

        //                    PathLine {
        //                        x: 3000
        ////                        relativeX: 3000 /** pathView.model.count*/  // all items in lines
        //                        relativeY: 0
        //                    }
        //                }
        //            }

        Timer {
            id: swipeTimer
            interval: 2000
            repeat: true
            onTriggered: {
//                console.log("§truggersd")
//                pathView.currentIndex += 1
            }
        }
    }

    ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectangle.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.rightMargin: 5
        anchors.leftMargin: 5

    }

}
