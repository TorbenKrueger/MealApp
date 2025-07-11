import QtQuick 2.0
import QtQuick.Controls 2.0

import QtQuick.Layouts 1.0

import CModel 1.0

import "../basic"

Rectangle {
    id: root
    height: parent.height
    width: parent.width
    color: "transparent"

    property bool asc: radioButtonAsc.checked
    property string orderType: "name"

    property string spacer: "        "
    property bool isCheckAll: true//checkAll.checked

    property var categories: []

    function categoriesChange()
    {
        categories = []

        if (checkBoxLunch.checked)
            categories.push(0)

        if (checkBoxDessert.checked)
            categories.push(1)

        if (checkBoxVeg.checked)
            categories.push(2)

        if (checkBoxCanVeg.checked)
            categories.push(3)

//        if (categories != []) {
//            if (categories.length == 4)
//                checkAll.checked = true
//            else
//                checkAll.checked = false
////                checkAll.checkState = Qt.PartiallyChecked
//        }
    }

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        anchors.top: parent.top
        anchors.bottom: buttonOk.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: primaryColor
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: buttonOk.bottom
        anchors.bottom: parent.bottom
        onClicked: {
            root.visible = false
        }
        Rectangle {
            id: bgOpa
            anchors.fill: parent
            opacity: 0.95
            color: primaryColor
        }
    }

    GroupBox {
        id: sort
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        contentHeight: 60
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.topMargin: 10
        title: "Sortierung"

        background: Rectangle {
            y: parent.topPadding - parent.bottomPadding
            width: parent.width
            height: parent.height - parent.topPadding + parent.bottomPadding
            color: "transparent"
            border.color: highlightColor
            radius: 20
        }
        label: Label {
            x: parent.leftPadding
            width: parent.availableWidth
            text: parent.title
            color: textColor
            elide: Text.ElideRight
            font.pointSize: headLinePointSize
        }

        RadioButton {
            id: radioButtonAsc
            text: qsTr("Aufsteigend")
            checked: mealList.orderAsc
            font.pointSize: stdPointSize
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins: 15

            indicator: Rectangle {
                implicitWidth: 26
                implicitHeight: 26
                x: radioButtonAsc.leftPadding
                y: parent.height / 2 - height / 2
                radius: 13
                border.color: highlightColor

                Rectangle {
                    width: 14
                    height: 14
                    x: 6
                    y: 6
                    radius: 7
                    color: secondaryColor
                    visible: radioButtonAsc.checked
                }
            }
            contentItem: Text {
                text: spacer + radioButtonAsc.text
                font: radioButtonAsc.font
                opacity: enabled ? 1.0 : 0.3
                color: textColor
                verticalAlignment: Text.AlignVCenter
            }
        }

        RadioButton {
            id: radioButtonDesc
            checked: !mealList.orderAsc
            text: qsTr("Absteigend")
            font.pointSize: stdPointSize
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 58
            anchors.margins: radioButtonAsc.anchors.margins

            indicator: Rectangle {
                implicitWidth: 26
                implicitHeight: 26
                x: radioButtonDesc.leftPadding
                y: parent.height / 2 - height / 2
                radius: 13
                border.color: highlightColor

                Rectangle {
                    width: 14
                    height: 14
                    x: 6
                    y: 6
                    radius: 7
                    color: secondaryColor
                    visible: radioButtonDesc.checked
                }
            }
            contentItem: Text {
                text: spacer + radioButtonDesc.text
                font: radioButtonDesc.font
                opacity: enabled ? 1.0 : 0.3
                color: textColor
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    ButtonGroup {
        buttons: [radioButtonName, radioButtonCreate, radioButtonRating, radioButtonCooked]
    }

    GroupBox {
        id: sortKind
        title: "Sortieren nach"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: sort.bottom
        anchors.topMargin: 15
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        contentHeight: 60 * 2

        background: Rectangle {
            y: parent.topPadding - parent.bottomPadding
            width: parent.width
            height: parent.height - parent.topPadding + parent.bottomPadding
            color: "transparent"
            border.color: highlightColor
            radius: 20
        }
        label: Label {
            x: parent.leftPadding
            width: parent.availableWidth
            text: parent.title
            color: textColor
            elide: Text.ElideRight
            font.pointSize: headLinePointSize
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 60

            RadioButton {
                id: radioButtonName
                text: qsTr("Name")
                checked: mealList.orderType == "name"
                font.pointSize: stdPointSize

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: radioButtonAsc.anchors.margins

                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: radioButtonName.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 13
                    border.color: highlightColor

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 7
                        color: secondaryColor
                        visible: radioButtonName.checked
                    }
                }
                contentItem: Text {
                    text: spacer + radioButtonName.text
                    font: radioButtonName.font
                    opacity: enabled ? 1.0 : 0.3
                    color: textColor
                    verticalAlignment: Text.AlignVCenter
                }
                onCheckedChanged: {
                    if (checked)
                        orderType = "name"
                }
            }
            RadioButton {
                id: radioButtonCreate
                checked: mealList.orderType == "creationdate"
                text: qsTr("Erstellungsdatum")
                font.pointSize: stdPointSize

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.margins: radioButtonAsc.anchors.margins

                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: radioButtonCreate.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 13
                    border.color: highlightColor

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 7
                        color: secondaryColor
                        visible: radioButtonCreate.checked
                    }
                }
                contentItem: Text {
                    text: spacer + radioButtonCreate.text
                    font: radioButtonCreate.font
                    opacity: enabled ? 1.0 : 0.3
                    color: textColor
                    verticalAlignment: Text.AlignVCenter
                }
                onCheckedChanged: {
                    if (checked)
                        orderType = "creationdate"
                }
            }
        }
        Item  {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 60

            RadioButton {
                id: radioButtonCooked
                checked: mealList.orderType == "lastcooked"
                text: qsTr("zul. gekocht")
                font.pointSize: stdPointSize

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: radioButtonAsc.anchors.margins

                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: radioButtonCooked.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 13
                    border.color: highlightColor

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 7
                        color: secondaryColor
                        visible: radioButtonCooked.checked
                    }
                }
                contentItem: Text {
                    text: spacer + radioButtonCooked.text
                    font: radioButtonCooked.font
                    opacity: enabled ? 1.0 : 0.3
                    color: textColor
                    verticalAlignment: Text.AlignVCenter
                }
                onCheckedChanged: {
                    if (checked)
                        orderType = "lastcooked"
                }
            }
            RadioButton {
                id: radioButtonRating
                checked: mealList.orderType == "rating"
                text: qsTr("Bewertung")
                font.pointSize: stdPointSize

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 64
                anchors.margins: radioButtonAsc.anchors.margins

                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: radioButtonRating.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 13
                    border.color: highlightColor

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 7
                        color: secondaryColor
                        visible: radioButtonRating.checked
                    }
                }
                contentItem: Text {
                    text: spacer + radioButtonRating.text
                    font: radioButtonRating.font
                    opacity: enabled ? 1.0 : 0.3
                    color: textColor
                    verticalAlignment: Text.AlignVCenter
                }
                onCheckedChanged: {
                    if (checked)
                        orderType = "rating"
                }
            }
        }
    }
    GroupBox {
        id: categoriesGroup
        title: "Kategorien"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: sortKind.bottom
        anchors.topMargin: 15
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        contentHeight: 60 * 2

        background: Rectangle {
            y: parent.topPadding - parent.bottomPadding
            width: parent.width
            height: parent.height - parent.topPadding + parent.bottomPadding
            color: "transparent"
            border.color: highlightColor
            radius: 20
        }
        label: Label {
            x: parent.leftPadding
            width: parent.availableWidth
            text: parent.title
            color: textColor
            elide: Text.ElideRight
            font.pointSize: headLinePointSize
            QmlCheckBox {
                id: checkAll
                y: -10
                x: checkBoxDessert.x
                text: "Alles wählen"
                txtColor: textColor
                borderColor: highlightColor
                checkColor: secondaryColor
                uncheckColor: "white"
//                onCheckStateChanged: {
//                    if (checkAll.checkState != Qt.PartiallyChecked) {
//                        checkBoxLunch.checked = checked
//                        checkBoxDessert.checked = checked
//                        checkBoxVeg.checked = checked
//                        checkBoxCanVeg.checked = checked
//                    }
//                }
            }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 60

            QmlCheckBox {
                id: checkBoxLunch
                checked: mealList.lunch
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: radioButtonAsc.anchors.margins

                txtColor: textColor
                borderColor: highlightColor
                checkColor: secondaryColor
                uncheckColor: "white"

                text: "Mittagessen"
                onCheckStateChanged: {
                    mealList.lunch = checked
                    categoriesChange()
                }
            }
            QmlCheckBox {
                id: checkBoxDessert
                checked: mealList.dessert
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 101
                anchors.margins: radioButtonAsc.anchors.margins

                txtColor: textColor
                borderColor: highlightColor
                checkColor: secondaryColor
                uncheckColor: "white"

                text: "Nachtisch"
                onCheckStateChanged: {
                    mealList.dessert = checked
                    categoriesChange()
                }
            }
        }
        Item  {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 60

            QmlCheckBox {
                id: checkBoxVeg
                checked: mealList.vegetarian
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: radioButtonAsc.anchors.margins

                txtColor: textColor
                borderColor: highlightColor
                checkColor: secondaryColor
                uncheckColor: "white"

                text: "Vegetarisch"
                onCheckStateChanged: {
                    mealList.vegetarian = checked
                    categoriesChange()
                }
            }
            QmlCheckBox {
                id: checkBoxCanVeg
                checked: mealList.canVegetarian
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.margins: radioButtonAsc.anchors.margins

                txtColor: textColor
                borderColor: highlightColor
                checkColor: secondaryColor
                uncheckColor: "white"

                text: "Geht auch vegetarisch"
                onCheckStateChanged: {
                    mealList.canVegetarian = checked
                    categoriesChange()
                }
            }
        }
    }
    QmlButton {
        id: buttonOk
        orgColor: secondaryColor
        height: 60
        anchors.left: sort.left
        anchors.right: sort.right
        anchors.top: categoriesGroup.bottom
        anchors.topMargin: 15
        border.color: highlightColor
        txtColor: textColor
        text: "Anwenden"
        onClicked: {
            mealList.order(orderType, asc, checkBoxLunch.checked, checkBoxDessert.checked, checkBoxVeg.checked, checkBoxCanVeg.checked,)
            root.visible = false
        }
    }
}

