import QtQuick 2.0
import QtQuick.Controls 2.0

import CModel 1.0

Rectangle {
    id: root
    color: "transparent"
    property var obj: null
    property bool desEnable: false
    property alias currentIndex: swipeView.currentIndex
    property alias description: descript
    property bool swipeEnable: true

    property bool onEditing: false

    height: 400
    width: 300

    Rectangle { //bg
        color: primaryColor
        z: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 0
        anchors.top: parent.top
        anchors.bottom: swipeView.top
    }

    Row {
        id: indicator
        z: 2
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 0
        height: 60
        Rectangle {
            id: page1
            width: parent.width / 2 - 2.5
            height: 60
            color: "transparent"//secondaryColor
            radius: 20

            Text {
                anchors.centerIn: parent
                text: "Zutaten"
                font.pointSize: headLinePointSize
                color: textColor
                font.bold: true
                opacity: swipeView.currentIndex === 0 ? 1 : 0.45
            }

            opacity: swipeView.currentIndex === 0 ? 1 : 0.45
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    swipeView.currentIndex = 0
                }
            }
        }

        Rectangle {
            id: page2
            width: parent.width / 2 - 2.5
            height: 60
            color: "transparent"//secondaryColor
            radius: 20
            opacity: swipeView.currentIndex === 1 ? 1 : 0.45

            Text {
                anchors.centerIn: parent
                text: "Beschreibung"
                font.pointSize: headLinePointSize
                color: textColor
                font.bold: true
                opacity: swipeView.currentIndex === 1 ? 1 : 0.45
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    swipeView.currentIndex = 1
                }
            }
        }
    }

    SwipeView {
        id: swipeView
        interactive: swipeEnable
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: indicator.bottom
        anchors.margins: 5
        anchors.topMargin: -15
        anchors.bottomMargin: 10
        currentIndex: 0
        spacing: 10

        Rectangle {
            border.color: highlightColor
            color: secondaryColor
            radius: 20
            anchors.topMargin: 30
            ListView {
                id: items
                anchors.fill: parent
                spacing: 1
                delegate: itemDel
                model: MealPropertyModel {
                    id: propertyModel
                    list: obj == null ? null : obj.properties
                }
            }
        }

        Rectangle {
            color: "transparent"
            Rectangle {
                anchors.fill: parent
                border.color: highlightColor
                color: secondaryColor
                radius: 20
            }

            Flickable {
                id: flickable
                anchors.fill: parent
                flickableDirection: Flickable.VerticalFlick
                anchors.margins: 10
//                anchors.bottomMargin: !onEditing ? 10 : 1000
//                anchors.topMargin: !onEditing ? 10 : -1000

                TextArea.flickable: TextArea {
                    z: -2
                    id: descript
                    enabled: desEnable
                    text: obj == null ? "" : obj.description
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    font.pointSize: stdPointSize
                    color: textColor
//                    anchors.fill: parent
//                    onEditingFinished: {
//                        onEditing = false
//                    }
//                    onActiveFocusChanged: onEditing = activeFocus
                }

                ScrollBar.vertical: ScrollBar {}
            }
        }
    }

    Component  {
        id: itemDel
        Rectangle {
            width: parent.width
            height: 40
            radius: 15
            color: "transparent"
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                //                border.color: highlightColor
                color: primaryColor
                radius: 20
            }
            Label {
                x: 10
                width: parent.width - x
                height: parent.height
                text: (quantity * (comboBox.currentIndex + 1)) + " " + unit + " " + name
                verticalAlignment: Text.AlignVCenter
                color: textColor
                font.pointSize: stdPointSize
            }
            MouseArea {
                anchors.fill: parent
                onPressAndHold: if(desEnable) obj.removeProperty(uuid)
            }
        }
    }
}
