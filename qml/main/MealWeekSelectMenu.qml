import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle {
    id: root
    color: "transparent"

    property var mealObject: null

    property string uuid: ""
    property string mealName: "TestMeal"
    property bool blockSignal: false
    property date currentDate: new Date
    property int persCount: 2

    onMealObjectChanged: {
        if (mealObject == null || mealObject == "" || mealObject == undefined)
            return

        if (uuid != mealObject.uuid)
            uuid = mealObject.uuid

        if (mealObject != null) {
            var list = MealWeekView.list
            for (var i = 0; i < 7; i++) {
                for (var k = 0; k < 3; k++) {
                    var tmpUuid = list[i][k].uuid
                    setMealWeekSelect(i, k, tmpUuid)
                }
            }
        }
    }

    function checkBoxStateChanged(checked, day, time)
    {
        if (blockSignal)
            return

        if (checked) {
            if (mealObject == null)
                return

            MealWeekView.setMeal(day, time, mealObject)
        }
        else {
            MealWeekView.removeMeal(day, time)
        }
    }


    function setMealWeekSelect(day, time, name)
    {
        blockSignal = true
        var obj = eval("checkBox" + day + time)
        if (name == "") {
            obj.checkState = Qt.Unchecked
        }
        else if (name == uuid) {
            obj.checkState = Qt.Checked
        }
        else {
            obj.checkState = Qt.PartiallyChecked
        }
        blockSignal = false
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.visible = false
        }
    }

    Rectangle {
        id: rectangle
        color: primaryColor
        height: 500
        width: 300
        anchors.right: parent.right
        anchors.top: parent.top

        GridLayout {
            id: grid
            anchors.fill: parent
            rows: 7
            columns: 4
            anchors.margins: 5

            Label {
                id: label0
                color: "#ffffff"
                text: qsTr("Wochentag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
            }

            Label {
                id: label1
                color: "#ffffff"
                text: qsTr("Morgens")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
            }

            Label {
                id: label2
                color: "#ffffff"
                text: qsTr("Mittags")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
            }

            Label {
                id: label3
                color: "#ffffff"
                text: qsTr("Abends")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
            }

            Label {
                color: "#ffffff"
                text: qsTr("Montag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox00
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 0, 0)
            }
            CheckBox {
                id: checkBox01
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 0, 1)
            }
            CheckBox {
                id: checkBox02
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 0, 2)
            }

            Label {
                color: "#ffffff"
                text: qsTr("Dienstag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox10
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 1, 0)
            }
            CheckBox {
                id: checkBox11
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 1, 1)
            }
            CheckBox {
                id: checkBox12
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 1, 2)
            }

            Label {
                color: "#ffffff"
                text: qsTr("Mittwoch")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox20
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 2, 0)
            }
            CheckBox {
                id: checkBox21
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 2, 1)
            }
            CheckBox {
                id: checkBox22
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 2, 2)
            }

            Label {
                color: "#ffffff"
                text: qsTr("Donnerstag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox30
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 3, 0)
            }
            CheckBox {
                id: checkBox31
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 3, 1)
            }
            CheckBox {
                id: checkBox32
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 3, 2)
            }

            Label {
                color: "#ffffff"
                text: qsTr("Freitag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox40
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 4, 0)
            }
            CheckBox {
                id: checkBox41
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 4, 1)
            }
            CheckBox {
                id: checkBox42
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 4, 2)
            }

            Label {
                color: "#ffffff"
                text: qsTr("Samstag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox50
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 5, 0)
            }
            CheckBox {
                id: checkBox51
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 5, 1)
            }
            CheckBox {
                id: checkBox52
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 5, 2)
            }

            Label {
                color: "#ffffff"
                text: qsTr("Sonntag")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: stdPointSize
                Layout.fillWidth: true
                font.bold: currentDate.toLocaleDateString(Qt.locale("de_DE"),'dddd') == text
            }

            CheckBox {
                id: checkBox60
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 6, 0)
            }
            CheckBox {
                id: checkBox61
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 6, 1)
            }
            CheckBox {
                id: checkBox62
                Layout.fillWidth: true
                onCheckStateChanged: checkBoxStateChanged(checked, 6, 2)
            }
        }
    }
}
