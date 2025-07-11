import QtQuick 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import CModel 1.0

import "../basic"
import "../mealoverview"

Item {
    id: item1

    property var object: null

    property int marginPx: 10
    property bool preview: false
    property bool filterBy: false

    readonly property string tmpPicPath: cam.tmpPath

    property string uuid: ""

    function setEdit(object, index)
    {
        item1.object = object
        uuid = object.uuid
        comboBox.currentIndex = object.personCount - 1
        comboBoxCat.currentIndex = object.categorie
        propertyOverview.currentIndex = index
        textFieldName.text = object.name

        image.source = item1.object.picturePath
        image.imgRotation = item1.object.pictureRotation
    }

    Rectangle {
        anchors.fill: parent
        color: primaryColor
    }

    NewCamera {
        id: cam
        anchors.fill: parent
        visible: false
        z: 10
        onPictureTaken: {
            if (item1.object == null)
                item1.object = mealList.createAndAppend("")

            item1.object.savePicture(path)
//            image.source = item1.object.picturePath
            image.source = "file://" + path
        }
    }

    FileDialog {
        id: fileDialog
        title: "Bild auswählen"
        nameFilters: ["Image Files (*.jpg *.png *.gif)"]
        //folder: Qt.resolvedUrl("file:///storage/emulated/0/DCIM/IMG_00000014.jpg")
        folder: shortcuts.pictures
        onAccepted: {
            var fileName = mealList.formatFile(fileUrl)
            object.picturePath = fileName;
            image.source = fileName
            visible = false;
        }
        onRejected: {
            visible = false;
        }
    }

    TextField {
        id: textFieldName
        x: 5
        y: 5
        z: 5
        height: 60
        maximumLength: 32
        placeholderText: qsTr("Name des Gerichtes")
        width: parent.width - 10
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
            button.forceActiveFocus()
            if (object == null && text != "") {
                object = mealList.createAndAppend(text)
            }
        }
    }


    Row {
        id: imgContainerRow
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textFieldName.bottom
        anchors.topMargin: 5
        anchors.margins: 5
        height: parent.width / 2

        QmlImage {
            id: image

            width: parent.width / 2 - 2.5
            height: imgContainerRow.height
            onClicked: {
                cam.visible = true
            }
            onPressAndHold: {
                fileDialog.visible = true
            }
        }

        Item {
            width: parent.width / 2 - 2.5
            height: imgContainerRow.height

            ComboBox {
                id: comboBoxCat
                z: 3
                visible: !preview
                height: 60
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 4
                model: ["Mittagessen", "Nachtisch", "Vegetarisch", "Geht auch vegetarisch"]
                currentIndex: try {object.categorie}catch(e){return 0}
                font.bold: false
                font.pointSize: stdPointSize

                delegate: ItemDelegate {
                    width: comboBoxCat.width - 2
                    background: Rectangle{
                        radius: 20
                        color: secondaryColor
                    }

                    contentItem: Text {
                        text: modelData
                        color: textColor
                        font: comboBoxCat.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle  {
                        anchors.fill: parent
                        color: primaryColor
                        visible: comboBoxCat.highlightedIndex === index
                        radius: 20
                    }
                    highlighted: comboBoxCat.highlightedIndex === index
                }

                contentItem: Text {
                    text: "   " + comboBoxCat.displayText
                    font: comboBoxCat.font
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
                    y: comboBoxCat.height + 5
                    width: comboBoxCat.width
                    height: 346
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
                        model: comboBoxCat.popup.visible ? comboBoxCat.delegateModel : null
                        currentIndex: comboBoxCat.highlightedIndex
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

            ComboBox {
                id: comboBox
                z: 3
                visible: !preview
                height: 60
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: comboBoxCat.bottom
                anchors.topMargin: 8

                model: ["1 Portion", "2 Portionen", "3 Portionen", "4 Portionen", "5 Portionen", "6 Portionen", "7 Portionen", "8 Portionen"]
                currentIndex: object == null ? 1 : object.personCount - 1
                font.bold: false
                font.pointSize: stdPointSize

                delegate: ItemDelegate {
                    width: comboBox.width - 2
                    background: Rectangle{
                        radius: 20
                        color: secondaryColor
                    }

                    contentItem: Text {
                        text: modelData
                        color: textColor
                        font: comboBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle  {
                        anchors.fill: parent
                        color: primaryColor
                        visible: comboBox.highlightedIndex === index
                        radius: 20
                    }
                    highlighted: comboBox.highlightedIndex === index
                }

                contentItem: Text {
                    text: "   " + comboBox.displayText
                    font: comboBox.font
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
                    y: comboBox.height + 5
                    width: comboBox.width
                    height: 346
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
                        model: comboBox.popup.visible ? comboBox.delegateModel : null
                        currentIndex: comboBox.highlightedIndex
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
                    if (object == null && textFieldName.text == "")
                        return

                    if (object == null )
                        object = mealList.createAndAppend(textFieldName.text)

                    if (object.personCount - 1 != currentIndex)
                        object.personCount = currentIndex + 1
                }
            }

            QmlButton {
                id: saveBtn
                anchors.top: comboBox.bottom
                anchors.topMargin: 8
                anchors.left: parent.left
                anchors.right: parent.right
                text: "Speichern"
                txtColor: textColor
                orgColor: secondaryColor
                font.pointSize: stdPointSize
                font.bold: false
                onClicked: {
                    if (textFieldName.text == "")
                        return

                    showFeedbackLabel("Wird gespeichert...", 2000)

                    if (object == null)
                        object = mealList.createAndAppend(textFieldName.text)

                    object.name = textFieldName.text
                    object.pictureRotation = image.imgRotation
                    object.description = propertyOverview.description.text
                    object.categorie = comboBoxCat.currentIndex

                    object.save()

                    showFeedbackLabel("Gespeichert", 2000)

                    textFieldName.text = ""
                    comboBox.currentIndex = 1
                    comboBoxCat.currentIndex = 0
                    textField.text = ""
                    propertyOverview.description.text = ""
                    propertyOverview.currentIndex = 0
                    item1.visible = false
                    object = null

                    navMasterButton.state = 0
                }
            }
        }
    }

    QmlButton {
        id: button
        txtColor: textColor
        orgColor: secondaryColor
        height: 60
        width: 120
        anchors.top: imgContainerRow.bottom
        anchors.right: parent.right
        anchors.rightMargin: marginPx
        anchors.topMargin: marginPx
        text: qsTr("Hinzufügen")
        onClicked: {
            propertyOverview.currentIndex = 0

            if (object == null) {
                object = mealList.createAndAppend(textFieldName.text)
            }
            var ret = object.addProperty(textField.text, comboBox.currentIndex + 1)
            if (ret)
                textField.text = ""

            textField.forceActiveFocus()
        }
    }

    TextField {
        id: textField
        anchors.top: imgContainerRow.bottom
        anchors.left: parent.left
        anchors.right: button.left
        anchors.margins: marginPx
        placeholderText: qsTr("Anzahl Einheit Name")
        text: "100 Gramm Paprika"
        color: textColor
        height: 60
        font.pointSize: stdPointSize
        background: Rectangle {
            color: secondaryColor
            border.color: highlightColor
            radius: 20
        }
    }

    Connections {
        target: root
        onKeyBoardVisibleChanged: {
            if (visible) {
                propertyOverview.z = 100
                propertyOverview.anchors.topMargin = -280
            }
            else {
                propertyOverview.z = 0
                propertyOverview.anchors.topMargin = 0
            }
        }
    }

    MealPropertyVisiElement {
        id: propertyOverview
        desEnable: true
        obj: object
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: textField.bottom
        anchors.topMargin: 0
        anchors.margins: 5
    }
}
