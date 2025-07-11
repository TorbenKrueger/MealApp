import QtQuick 2.15
import QtQuick.Controls 2.15

import CModel 1.0

Rectangle {
    id: root
    color: primaryColor

    function undo() {shoppModel.list.undoLastItem()}
    function showAdd() {textField.visible = true}

    TextEdit {
        id: copyToClipboard
        visible: false
    }

    ListView {
        id: items
        anchors.fill: parent
        anchors.margins: 5
        anchors.bottomMargin: 11
        spacing: 5
        delegate: QmlListDelegateMealProperties {
            height: 60
            width: items.width
            color: secondaryColor
            border.color: highlightColor
            onItemClicked: {
                navMasterButton.state = 7
                shoppModel.list.removeItem(index)
                timer.start()
            }
            onItemPressAndHold: {
                copyToClipboard.text = quantity + " " + unit + " " + name
                copyToClipboard.selectAll()
                copyToClipboard.copy()
                showFeedbackLabel("In die Zwischenablage kopiert", 2000)
            }
        }

        model: ShoppingListModel {
            id: shoppModel
            list: shoppingList
        }
    }

    QmlButton {
        id: revert
        txtColor: textColor
        orgColor: secondaryColor
        z: 10
        text: "Hinzufügen"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -60
        onClicked: {
            if (text != "Hinzufügen") {
                text = "Hinzufügen"
                shoppModel.list.undoLastItem()
            }
            else {
                textField.visible = true
            }
        }
    }

    Timer {
        id: timer
        repeat: false
        interval: 5000
        onTriggered: {
            navMasterButton.state = 6
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "white"
        opacity: 0.75
        visible: textField.visible
        MouseArea {
            anchors.fill: parent
            onClicked: {
                textField.text = ""
                textField.visible = false
            }
        }
    }

    TextField {
        id: textField
        z:1
        visible: false
        anchors.centerIn: parent
        width: root.width - 10
        anchors.left: root.left
        anchors.leftMargin: 5
        placeholderText: qsTr("Anzahl Einheit Name")
        text: ""
        color: textColor
        height: 60
        font.pointSize: stdPointSize
        background: Rectangle {
            color: secondaryColor
            border.color: highlightColor
        }
    }
    QmlButton {
        id: ok
        text: "Hinzufügen"
        visible: textField.visible
        anchors.top:textField.bottom
        anchors.topMargin: textField.anchors.leftMargin
        anchors.left: textField.left
        width: textField.width / 2
        onClicked: {
            shoppModel.list.appendItem(textField.text)
            textField.visible = false
        }
    }
    QmlButton {
        id: nok
        text: "Abbrechen"
        visible: textField.visible
        anchors.top:textField.bottom
        anchors.topMargin: textField.anchors.leftMargin
        anchors.right: textField.right
        width: textField.width / 2
        onClicked: {
            textField.text = ""
            textField.visible = false
        }
    }
}
