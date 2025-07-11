import QtQuick 2.15
import QtQuick.Controls 2.15

import QtQuick.Layouts 1.0

import CModel 1.0

import "../basic"
import "../main"

Rectangle {
    id: root
    height: parent.height
    width: parent.width

    color: primaryColor

    property string spacer: "          "

    function updateView()
    {
        for (var index = 0; index < 4; index++) {
            switch (index) {
            case 0:
                var color = userColors[index]
                col0.color = color
                txt0.color = lightDark(color, "white", "black")
                break;
            case 1:
                color = userColors[index]
                col1.color = color
                txt1.color = lightDark(color, "white", "black")
                break;
            case 2:
                color = userColors[index]
                col2.color = color
                txt2.color = lightDark(color, "white", "black")
                break;
            case 3:
                color = userColors[index]
                col3.color = color
                txt3.color = lightDark(color, "white", "black")
                break;
            }
        }
    }

    QmlColorDialog {
        id: colorDialog
        property int index: 0
        onColorSelected:{
            userColors[index] = color
            setStyle()
            switch (index) {
            case 0:
                col0.color = color
                txt0.color = lightDark(color, "white", "black")
                break;
            case 1:
                col1.color = color
                txt1.color = lightDark(color, "white", "black")
                break;
            case 2:
                col2.color = color
                txt2.color = lightDark(color, "white", "black")
                break;
            case 3:
                col3.color = color
                txt3.color = lightDark(color, "white", "black")
                break;
            }
        }
    }

    function lightDark(background, lightColor, darkColor) {
        return !isDarkColor(background) ? darkColor : lightColor
    }

    function isDarkColor(background) {
        var temp = Qt.darker(background, 1) //Force conversion to color QML type object
        var a = 1 - ( 0.299 * temp.r + 0.587 * temp.g + 0.114 * temp.b);
        return temp.a > 0 && a >= 0.3
    }

    MessageBox {
        id: resetAll1
        z: 10
        visible: false
        anchors.fill: parent
        title: "Alles löschen?"
        text: "Wollen Sie wirklich alle Daten löschen?\nDieser Vorgang kann nicht rückgänig gemacht werden"
        okText: "Ja"
        nokText: "Nein"
        onAccepted: resetAll2.show()
    }
    MessageBox {
        id: resetAll2
        z: 11
        visible: false
        anchors.fill: parent
        title: "Bestätigen: Alles löschen?"
        text: "Es werden alle Daten unwiderruflich gelöscht!\nDie App beendet sich danach!"
        okText: "Fortfahren"
        nokText: "Abbrechen"
        onAccepted: mealList.resetAll()
    }

    Grid {
        z: 2
        anchors.fill: parent
        anchors.bottomMargin: 60
        anchors.topMargin: 10
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        columns: 1
        rows: 5
        rowSpacing: 20
        spacing: 5

        Grid {
            Layout.fillWidth: true
            spacing: 5
            columns: 3
            rows: 1

            ComboBox {
                id: comboStyleMode
                z: 3
                height: 60
                model: ["Darkmode", "Lightmode", "Benutzerdefinert"]
                width: root.width / 2 - 7.5
                currentIndex: styleMode
                font.bold: false
                font.pointSize: stdPointSize

                delegate: ItemDelegate {
                    width: comboStyleMode.width - 2
                    background: Rectangle{
                        radius: 20
                        color: secondaryColor
                    }

                    contentItem: Text {
                        text: modelData
                        color: textColor
                        font: comboStyleMode.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle  {
                        anchors.fill: parent
                        color: primaryColor
                        visible: comboStyleMode.highlightedIndex === index
                        radius: 20
                    }
                    highlighted: comboStyleMode.highlightedIndex === index
                }
                contentItem: Text {
                    text: "   " + comboStyleMode.displayText
                    font: comboStyleMode.font
                    color: textColor
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 60
                    color: secondaryColor
                    radius: 20
                    border.color: highlightColor
                }
                popup: Popup {
                    y: comboStyleMode.height + 5
                    width: comboStyleMode.width
                    height: 120
                    padding: 1

                    Rectangle {
                        anchors.fill: parent
                        color: secondaryColor
                        opacity: 0.8
                        z: -1
                        radius: 20
                    }

                    contentItem: ListView {
                        interactive: false
                        implicitHeight: contentHeight
                        model: comboStyleMode.popup.visible ? comboStyleMode.delegateModel : null
                        currentIndex: comboStyleMode.highlightedIndex
                        delegate: Component {
                            Rectangle {
                                width: 100
                                height: 60
                                visible: true
                                radius: 20
                                color: "transparent"
                                Label {
                                    x: 10
                                    height: parent.height
                                    width: parent.width
                                    text: index
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: stdPointSize
                                    font.bold: true
                                    color: textColor
                                }
                            }
                        }
                    }

                    background: Rectangle {
                        border.color: highlightColor
                        color: secondaryColor
                        opacity: 0.75
                        radius: 20
                    }

                }
                onCurrentIndexChanged: {
                    styleMode = currentIndex
                    gridUserColor.visible = currentIndex == 2
                }
            }

            QmlButton {
                orgColor: secondaryColor
                txtColor: textColor
                border.color: highlightColor
                text: "App zurücksetzen"
                width: root.width / 2  - 7.5
                onClicked: resetAll1.show()
            }
        }
        Grid {
            id: gridUserColor
            visible: comboStyleMode.currentIndex == 2
            Layout.fillWidth: true
            spacing: 5
            columns: 2
            rows: 2

            Rectangle {
                id: col0
                radius: 15
                Text {
                    id: txt0
                    anchors.centerIn: parent
                    text: qsTr("Primär Farbe")
                    color: lightDark(mealList.primaryColor == "" ? "white" : mealList.primaryColor, "white", "black")
                    font.pointSize: stdPointSize
                }
                border.color: txt0.color
                height: 40
                width: root.width / 2 - 7.5
                color: mealList.primaryColor == "" ? "white" : mealList.primaryColor

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorDialog.setColor(mealList.primaryColor)
                        colorDialog.index = 0
                        colorDialog.show()
                    }
                }
            }
            Rectangle {
                id: col1
                radius: 15
                Text {
                    id: txt1
                    anchors.centerIn: parent
                    text: qsTr("Sekundär Farbe")
                    color: lightDark(mealList.secondaryColor == "" ? "white" : mealList.secondaryColor, "white", "black")
                    font.pointSize: stdPointSize
                }
                border.color: txt1.color
                height: 40
                width: root.width / 2 - 7.5
                color: mealList.secondaryColor == "" ? "white" : mealList.secondaryColor
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorDialog.setColor(mealList.secondaryColor)
                        colorDialog.index = 1
                        colorDialog.show()
                    }
                }
            }
            Rectangle {
                id: col2
                radius: 15
                Text {
                    id: txt2
                    anchors.centerIn: parent
                    text: qsTr("Text Farbe")
                    color: lightDark(mealList.textColor == "" ? "white" : mealList.textColor, "white", "black")
                    font.pointSize: stdPointSize
                }
                border.color: txt2.color
                height: 40
                width: root.width / 2 - 7.5
                color: mealList.textColor == "" ? "white" : mealList.textColor
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorDialog.setColor(mealList.textColor)
                        colorDialog.index = 2
                        colorDialog.show()
                    }
                }
            }
            Rectangle {
                id: col3
                radius: 15
                Text {
                    id: txt3
                    anchors.centerIn: parent
                    text: qsTr("Highlight Farbe")
                    color: lightDark(mealList.highlightColor == "" ? "white" : mealList.highlightColor, "white", "black")
                    font.pointSize: stdPointSize
                }
                border.color: txt3.color
                height: 40
                width: root.width / 2 - 7.5
                color: mealList.highlightColor == "" ? "white" : mealList.highlightColor
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorDialog.setColor(mealList.highlightColor)
                        colorDialog.index = 3
                        colorDialog.show()
                    }
                }
            }
        }
        Grid {
            Layout.fillWidth: true
            spacing: 5
            columns: 2
            rows: 1

            QmlButton {
                orgColor: secondaryColor
                txtColor: textColor
                border.color: highlightColor
                width: root.width / 2 - 7.5
                text: "Backup erstellen"
                onClicked: mealList.createBackup(backupName.text)
            }

            QmlButton {
                orgColor: secondaryColor
                txtColor: textColor
                border.color: highlightColor
                text: "Backup laden"
                width: root.width / 2 - 7.5
                onClicked: {
                    if (comboBackUp.currentIndex == 0)
                        mealList.loadBackup(backupName.textr)
                    else
                        mealList.loadBackup(comboBackUp.currentText)
                }
            }
        }
        Grid {
            Layout.fillWidth: true
            spacing: 5
            columns: 2
            rows: 1
            TextField {
                id: backupName
                height: 60
                width: root.width / 2 - 7.5
                placeholderText: qsTr("Name des Backups")
                text: "backup5"
                font.bold: true
                font.pointSize: stdPointSize
                color: textColor
                background: Rectangle {
                    color: secondaryColor
                    border.color: highlightColor
                    radius: 20
                    opacity: 0.8
                }
                onEditingFinished: {
                    gridUserColor.forceActiveFocus()
                }
            }
            ComboBox {
                id: comboBackUp
                z: 3
                height: 60
                //                model: mealList.backups
                model: ["", "backup1", "backup2", "backup3", "backup4", "backup5", "autoBackup"]
                width: root.width / 2 - 7.5
                currentIndex: styleMode
                font.bold: false
                font.pointSize: stdPointSize

                delegate: ItemDelegate {
                    width: comboBackUp.width - 2
                    background: Rectangle{
                        radius: 20
                        color: secondaryColor
                    }

                    contentItem: Text {
                        text: modelData
                        color: textColor
                        font: comboBackUp.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle  {
                        anchors.fill: parent
                        color: primaryColor
                        visible: comboBackUp.highlightedIndex === index
                        radius: 20
                    }
                    highlighted: comboBackUp.highlightedIndex === index
                }
                contentItem: Text {
                    text: "   " + comboBackUp.displayText
                    font: comboBackUp.font
                    color: textColor
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 60
                    color: secondaryColor
                    radius: 20
                    border.color: highlightColor
                }
                popup: Popup {
                    y: comboBackUp.height + 5
                    width: comboBackUp.width
                    height: 60 * comboBackUp.model.length
                    padding: 1

                    Rectangle {
                        anchors.fill: parent
                        color: secondaryColor
                        opacity: 0.8
                        z: -1
                        radius: 20
                    }

                    contentItem: ListView {
                        interactive: false
                        implicitHeight: contentHeight
                        model: comboBackUp.popup.visible ? comboBackUp.delegateModel : null
                        currentIndex: comboBackUp.highlightedIndex
                        delegate: Component {
                            Rectangle {
                                width: 100
                                height: 60
                                visible: true
                                radius: 20
                                color: "transparent"
                                Label {
                                    x: 10
                                    height: parent.height
                                    width: parent.width
                                    text: index
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: stdPointSize
                                    font.bold: true
                                    color: textColor
                                }
                            }
                        }
                    }

                    background: Rectangle {
                        border.color: highlightColor
                        color: secondaryColor
                        opacity: 0.75
                        radius: 20
                    }

                }
            }
        }
        Grid {
            Layout.fillWidth: true
            spacing: 5
            columns: 2
            rows: 1
            QmlButton {
                id: qmlButton
                orgColor: secondaryColor
                txtColor: textColor
                border.color: highlightColor
                width: root.width / 2 - 7.5
                text: "Daten zum Server senden"
                onClicked: {
                    mealList.sendAllToServer()
                }
            }
            QmlButton {
                id: qmlButtonGetData
                orgColor: secondaryColor
                txtColor: textColor
                border.color: highlightColor
                width: root.width / 2 - 7.5
                text: "Daten vom Server holen"
                onClicked: {
                    mealList.getAllFromServer()
                }
            }
        }

        Grid {
            Layout.fillWidth: true

            TextEdit {
                id: textEdit
                width: 173
                height: 60
                font.pixelSize: 12
                wrapMode: Text.WrapAnywhere
                text: "MealAppBackup.txt"
            }

            QmlButton {
                id: qmlButtonGetData1
                width: root.width / 2 - 7.5
                text: "Daten vom Server holen"
                border.color: highlightColor
                orgColor: secondaryColor
                txtColor: textColor
                onClicked: {
                    mealList.loadBackupFromServer(textEdit.text)
                }
            }

            spacing: 5
            columns: 2
            rows: 1
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.visible = false
        }

    }
}

