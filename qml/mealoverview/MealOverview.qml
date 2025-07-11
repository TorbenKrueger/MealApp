import QtQuick 2.2
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import CModel 1.0

import "../basic"
import "../main"

Rectangle {
    id: mealOverview
    visible: false
    color: primaryColor

    property string uuid: ""
    property var object: null

    property var weekObject: null

    property bool preview: false
    property bool showWeekOverview: true
    property bool showToogleViewAction: false

    onObjectChanged: {
        if (object == null)
            return

        ratingMenu.setRating(object.rating)
        image.source = object.picturePath
        image.imgRotation = object.pictureRotation
    }
    onWeekObjectChanged: {
        object = weekObject.object
    }

    function setMealWeekSelect(day, time, name)
    {
        mealWeekSelectMenu.setMealWeekSelect(day, time, name)
    }

    MessageBox {
        id: removePic
        z: 999
        title: "Löschen?"
        text: "Soll das Foto gelöscht werden?"
        okText: "Ja"
        nokText: "Nein"
        visible: false
        anchors.centerIn: parent
        onAccepted: {
            if (object != null) {
                object.savePicture("removePic")
                image.source = object.picturePath
                image.imgRotation = object.picturePath
            }
        }
    }

    Rectangle {
        id: showPicFull
        z: 100
        visible: false
        anchors.fill: parent
        opacity: 0.90
        color: primaryColor
    }
    MouseArea {
        z: 102
        visible: showPicFull.visible
        anchors.fill: showPicFull
        onClicked: showPicFull.visible = false
    }

    QmlImage {
        id: fullImg
        z: 101
        visible: showPicFull.visible
        anchors.fill: showPicFull
        anchors.margins: 5
        source: image.source
    }

    QuickMenu {
        z: 6
        radius: 20
        visible: showWeekOverview
        anchors.fill: parent
        defHeight: textLabel.height + 5
        defColor: primaryColor
        defTextColor: textColor
        defBorderColor: highlightColor
        defBgColor: secondaryColor
        anchors2.topMargin: 5
        anchors2.rightMargin: 5
        model: ["Kopieren", "Woche", "Bearbeiten", "Bild drehen", "Bild löschen"]
        font.bold: false
        font.pointSize: stdPointSize
        onTriggered: {
            if (index === 0) {
                copyToClipboard.text = object.getAllProperties()
                copyToClipboard.selectAll()
                copyToClipboard.copy()
                showFeedbackLabel("In die Zwischenablage kopiert", 2000)
            }
            else if (index === 1) {
                mealWeekSelectMenu.visible = true
            }
            else if (index === 2) {
                navMasterButton.clicked("")
                editMeal(object,  propertyOverview.currentIndex)
            }
            else if (index === 3) {
                image.imgRotation += 90
                if (image.imgRotation == 360)
                    image.imgRotation = 0

                while (image.imgRotation > 360)
                    image.imgRotation -= 360

                object.pictureRotation = image.imgRotation
            }
            else if (index === 4) {
                removePic.show()
            }
            else {

            }
        }
    }

    MouseArea {
        anchors.fill: parent
    }

    MealWeekSelectMenu {
        id: mealWeekSelectMenu
        mealObject: object == null ? "" : object
        uuid: object == null ? "" : object.uuid
        mealName: object == null ? "" : object.name
        anchors.fill: parent
        persCount: comboBox.currentIndex + 1
        z: 15
        visible: false
    }

    Label {
        id: textLabel
        x: 15
        y: 7.5
        z: 5
        width: 220
        height: 55
        verticalAlignment: Text.AlignVCenter
        text: object == null ? "" : object.name
        font.bold: true
        font.pointSize: headLinePointSize + 4
        color: textColor
    }
    Rectangle{
        z: 4
        y: 5
        height: textLabel.height + 5
        width: !showWeekOverview ? parent.width : parent.width - 50
        color: secondaryColor
        opacity: 0.75
        radius: 0
    }

    TextEdit {
        id: copyToClipboard
        visible: false
    }

    Row {
        id: imgContainerRow
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textLabel.bottom
        anchors.topMargin: 7.5
        anchors.margins: 5
        height: parent.width / 2

        QmlImage {
            id: image

            width: parent.width / 2 - 2.5
            height: imgContainerRow.height
            onClicked: {
                showPicFull.visible = true
            }
        }

        Item {
            width: parent.width / 2 - 2.5
            height: imgContainerRow.height

            Label {
                anchors.topMargin: 12.5
                anchors.left: parent.left
                anchors.right: parent.right
                text: try {object.categorieText()}catch(e){return "Fehler"}
                color: textColor
                font.pointSize: headLinePointSize
            }

            Rating {
                id: ratingMenu
                stars: 0
                size: 0.355
                anchors.topMargin: 22.5
                anchors.rightMargin: -10
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                onRatingChanged: {
                    object.rating = rating
                }
            }

            ComboBox {
                id: comboBox
                z: 3
                visible: !preview
                height: 60
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 77

                model: ["1 Portion", "2 Portionen", "3 Portionen", "4 Portionen", "5 Portionen", "6 Portionen", "7 Portionen", "8 Portionen"]
                currentIndex: object == null ? 1 : weekObject == null ? object.personCount - 1 : weekObject.personCount - 1
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
                    if (weekObject != null && weekObject.personCount - 1 != currentIndex) {
                        weekObject.personCount = currentIndex + 1
                        return;
                    }

                    if (object != null && object.personCount - 1 != currentIndex)
                        object.personCount = currentIndex + 1
                }
            }

            QmlButton {
                anchors.top: comboBox.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.right: parent.right
                text: "Wochenübersicht"
                txtColor: textColor
                orgColor: secondaryColor
                font.pointSize: stdPointSize
                font.bold: false
                onClicked: {
                    mealWeekSelectMenu.visible = true
                }
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        z: -1
        color: primaryColor
    }

    MealPropertyVisiElement {
        id: propertyOverview
        obj: object
        swipeEnable: !showToogleViewAction
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: imgContainerRow.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.topMargin: 1
        anchors.margins: 5
    }
}
