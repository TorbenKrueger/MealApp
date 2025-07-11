import QtQuick 2.0
import QtQuick.Controls 2.0

import CModel 1.0

import "../basic"
import "../mealoverview"

Rectangle {
    id: master
    objectName: "master"
    anchors.bottom: parent.bottom
    anchors.top: parent.top
    anchors.topMargin: -3
    color: "transparent"

    property int newListIndex: -1
    property int newSwipeIndex: -1

    function selectedMeal(obj)
    {
        navMasterButton.state = 4

        weekMealOverview.weekObject = obj
        weekMealOverview.visible = true

        masterView.interactive = false
    }

    function removeMeal(listIndex, swipeIndex, obj)
    {
        MealWeekView.removeMeal(listIndex, swipeIndex)
    }

    function hideWeekMealOverview() {weekMealOverview.visible = false}
    function hideMealSelectList(){mealSelectList.visible = false}

    ListView {
        id: view
        objectName: "view"
        visible: !weekMealOverview.visible
        anchors.fill: parent
        anchors.margins: 5
        anchors.topMargin: 8
        anchors.bottomMargin: 71
        delegate: listDelegate
        spacing: 5
        boundsBehavior: ListView.StopAtBounds
        model: ["Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag", "Sonntag"]
    }

    Component {
        id: listDelegate
        Rectangle {
            objectName: "listDelegate"
            property int idx: index
            id: bg
            color: secondaryColor
            height: master.height / 3
            width: view.width
            radius: 22
            Label {
                id: textLabel
                visible: true
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 6
                z: 5
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                height: 15
                text: view.model[index]
                font.bold: true
                font.pointSize: headLinePointSize
                color: textColor
            }

            SwipeView {
                id: swipeView
                anchors.topMargin: 26
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                WeekViewItem {
                    id: view1
                    listIndex: bg.idx
                    swipeIndex: 0
                    object: MealWeekView.list[listIndex][swipeIndex]
                    onSelectNewMeal: {
                        newListIndex = listIndex
                        newSwipeIndex = swipeIndex
                        mealSelectList.visible = true
                    }

                    onMealClicked: {
                        newListIndex = listIndex
                        newSwipeIndex = swipeIndex
                        selectedMeal(obj)
                    }
                    onMealRemoved: removeMeal(listIndex, 0, obj)
                }
                WeekViewItem {
                    id: view2
                    listIndex: bg.idx
                    swipeIndex: 1
                    object: MealWeekView.list[listIndex][swipeIndex]
                    onSelectNewMeal: {
                        newListIndex = listIndex
                        newSwipeIndex = swipeIndex
                        mealSelectList.visible = true
                    }

                    onMealClicked: {
                        newListIndex = listIndex
                        newSwipeIndex = swipeIndex
                        selectedMeal(obj)
                    }
                    onMealRemoved: removeMeal(listIndex, swipeIndex, obj)
                }
                WeekViewItem {
                    id: view3
                    listIndex: bg.idx
                    swipeIndex: 2
                    object: MealWeekView.list[listIndex][swipeIndex]
                    onSelectNewMeal: {
                        newListIndex = listIndex
                        newSwipeIndex = swipeIndex
                        mealSelectList.visible = true
                    }

                    onMealClicked: {
                        newListIndex = listIndex
                        newSwipeIndex = swipeIndex
                        selectedMeal(obj)
                    }
                    onMealRemoved: removeMeal(listIndex, swipeIndex, obj)
                }
            }
            PageIndicator {
                visible: swipeView.count > 1
                height: 50
                count: swipeView.count
                currentIndex: swipeView.currentIndex
                anchors.top: swipeView.top
                anchors.topMargin: -3
                width: swipeView.width + 1
                x: 3
                spacing: 1
                delegate: Rectangle {
                    width: parent.width / swipeView.count
                    height: parent.height
                    opacity: index === swipeView.currentIndex ? 1 : 0.5
                    color: secondaryColor
                    radius: 15
                    Text{
                        anchors.fill: parent
                        anchors.leftMargin: 1
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
//                        horizontalAlignment: swipeView.itemAt(index).object.name == "" ? Text.AlignHCenter : Text.AlignLeft
                        text: swipeView.itemAt(index).object.name == "" ? "-" : swipeView.itemAt(index).object.name
                        color: textColor
                        font.pointSize: 14 //stdPointSize - 2
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            swipeView.currentIndex = index
                        }
                    }
                }
            }
        }
    }

    MealOverview {
        id: weekMealOverview
        showWeekOverview: false
        anchors.fill: parent
        z: 9
        visible: false
        anchors.bottomMargin: 60
    }

    ListView {
        id: mealSelectList
        visible: false
        spacing: 5
        anchors.fill: parent
        anchors.topMargin: 8
        anchors.margins: 5
        anchors.bottomMargin: 71
        z: 9

        model: MealListModel {
            id: model
            list: mealList
        }
        delegate: QmlListDelegate {
            width: parent.width
            height: 60

            color: secondaryColor
            txtColor: textColor
            borderColor: highlightColor

            onClicked: {
                masterView.interactive = true
                navMasterButton.state = 1
                MealWeekView.setMeal(newListIndex, newSwipeIndex, uuid, -1)
                mealSelectList.visible = false
            }
        }
    }
    Rectangle {
        anchors.fill: mealSelectList
        visible: mealSelectList.visible
        color: primaryColor
        opacity: 0.75
        z: 8
    }
}
