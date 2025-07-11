import QtQuick 2.15
import QtQuick.Window 2.15

import QtQuick.Controls 2.15

import CModel 1.0

import "../basic"
import "../homepage"
import "../mealoverview"
import "../settings"
import "../weekview"

ApplicationWindow {
    id: root
    objectName: "root"
    width: 500
    height: 1083
    visible: true
    title: qsTr("Meals")
    color: primaryColor

    property int tmpHeight: 0
    onHeightChanged:  {
        if (tmpHeight >= height)
            keyBoardVisibleChanged(true)
        else
            keyBoardVisibleChanged(false)

        tmpHeight = height
    }

    signal keyBoardVisibleChanged(var visible)

    property int styleMode: 1

    property color primaryColor: "#121212"
    property color secondaryColor: "#262626"
    property color textColor: "white"
    property color highlightColor: "white"

    property int stdPointSize: 16
    property int headLinePointSize: 18

    property var userColors: ["", "", "", ""]

    property bool canClose: false

    property var swipeHomeObj: null

    onStyleModeChanged: {
        setStyle()
    }

    onClosing: {
        navMasterButton.forceActiveFocus()

        if (canClose) {
            close.accepted = true
        }
        else {
            if (navMasterButton.text == "Zurück" || masterView.currentIndex == 1) {
                if (navMasterButton.text == "Zurück") {
                    if (mealAdd.visible == true) {
                        goBackWithOutSave.show()
                    }
                    else {
                        navMasterButton.clicked("")
                    }
                }
                if (navMasterButton.state == 4 || navMasterButton.state == 5)
                    navMasterButton.clicked("")
            }
            else {

                // Nachricht
                canClose = true
                closeTimer.start()
            }
            close.accepted = false
        }
        console.log(close.accepted)
    }

    Timer {
        id: closeTimer
        interval: 750
        onTriggered: {
            canClose = false
            console.log("Timer end")
        }
    }

    Component.onCompleted: {
        loadStyle(mealList.styleMode, mealList.primaryColor, mealList.secondaryColor, mealList.textColor, mealList.highlightColor)
        if (!homepage.isCheckAll)
            showFeedbackLabel("Es werden nicht alle Gerichte angezeigt", 2000)
    }

    function loadStyle(style, col1, col2, col3, col4)
    {
        styleMode = style
        userColors[0] = col1
        userColors[1] = col2
        userColors[2] = col3
        userColors[3] = col4
        setStyle()
    }

    function setStyle()
    {
        switch (styleMode) {
        case 0: {               //org / darkmode
            primaryColor = "#121212"
            secondaryColor = "#262626"
            textColor = "white"
            highlightColor = "white"
            break;
        }
        case 1: {               // hell
            primaryColor = "#f2f2f2"
            secondaryColor = "#cccccc"
            textColor = "black"
            highlightColor = "black"
            break;
        }
        case 2: {
            primaryColor = userColors[0] == "" ? primaryColor : userColors[0]
            secondaryColor = userColors[1] == "" ? secondaryColor : userColors[1]
            textColor = userColors[2] == "" ? textColor : userColors[2]
            highlightColor = userColors[3] == "" ? highlightColor : userColors[3]
            break;
        }
        default: {
            console.log("Style nicht definert -> default Style")
            primaryColor = "#121212"
            secondaryColor = "#262626"
            textColor = "white"
            highlightColor = "white"
        }
        }
        mealList.primaryColor = primaryColor
        mealList.secondaryColor = secondaryColor
        mealList.textColor = textColor
        mealList.highlightColor = highlightColor
        mealList.styleMode = styleMode
    }

    function mealSelected(object)
    {
        overview.object = object
        overview.visible = true
        navMasterButton.state = 3
    }

    function setMealWeekSelect(day, time, name)
    {
        overview.setMealWeekSelect(day, time, name)
    }

    function setError(text)
    {
        showError.text = text
        showError.show()
    }

    function editMeal(object, index)
    {
        mealEdit.setEdit(object, index)
        mealEdit.visible = true
        navMasterButton.state = 2
    }

    function showFeedbackLabel(message, duration)
    {
        feedbackLabel.closeAfter = duration
        feedbackLabel.show(message)
    }

    function readyToRemove(object)
    {
        remove.obj = object
        remove.text = "Soll das Gericht <b>" + object.name + "</b> gelöscht werden?"
        remove.show()
    }

    function createSwipeView()
    {
        swipeHomeObj = Qt.createQmlObject("import QtQuick 2.15;
                        HomePageSwipeView {
                            id: homepageSwipeView;
                            anchors.fill: parent;
                            }", homepageContainer, "homepageSwipeViewObj" )
    }

    function newMealFromServer(uuid, name, date)
    {
        console.log("QML nächstetes essen")
        serverAdd.uuid = uuid
        serverAdd.text = "Soll das Gericht " + name + " erstellt am " + date + " hinzugefügt werden?\nUUID: " + uuid
        serverAdd.show()
    }

    MessageBox {
        id: serverAdd
        property string uuid: ""
        z: 999
        title: "Gericht vom Server hinzufügen?"
        text: ""
        okText: "Ja"
        nokText: "Nein"
        visible: false
        anchors.centerIn: parent
        onAccepted: {
            mealList.acceptedServerMeal(uuid)
        }
        onRejected: {
            mealList.nextServerMeal(uuid)
        }
    }

    MessageBox {
        id: remove
        property var obj: null
        z: 999
        title: "Löschen?"
        text: "Fehler bei der Auswahl des Gerichtes"
        okText: "Ja"
        nokText: "Nein"
        visible: false
        anchors.centerIn: parent
        onAccepted: {
            if (obj != null)
                obj.remove()
        }
    }

    MessageBox {
        id: showError
        z: 999
        title: "Fehler"
        visible: false
        onlyOkVisible: true
        anchors.centerIn: parent
    }

    MessageBox {
        id: goBackWithOutSave
        z: 999
        title: "Speichern"
        text: "Alle ungespeicherten Änderungen gehen verloren!"
        okText: "In Ordnung"
        nokText: "Abbrechen"
        visible: false
        onlyOkVisible: false
        anchors.centerIn: parent
        onAccepted: {
            navMasterButton.clicked("")
        }
    }

    MessageBox {
        id: messageBoxResetWeek
        z: 20
        title: "Bestätigen"
        text: "Sollen alle Gerichte aus der Wochenübersicht gelöscht werden?"
        okText: "Ja"
        nokText: "Nein"
        visible: false
        anchors.fill: parent
        onAccepted: {
            for (var i = 0; i < 7; i++) {
                for (var k = 0; k < 3; k++) {
                    MealWeekView.removeMeal(i, k)
                }
            }
        }
    }

    FeedBackLabel{
        id: feedbackLabel
        z: 999
        color: primaryColor
        txtColor: textColor
        border.color: "transparent"
        closeAfter: 2000
        width: parent.width / 1.25
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 120
    }

    QmlButton {
        id: navMasterButton
        property int state: 0
        txtColor: textColor
        orgColor: secondaryColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        font.pointSize: stdPointSize
        z: 2
        anchors.margins: 5

        text: "Hinzufügen"
        onStateChanged: {
            switch(state)
            {
            case 0: //default
            case 6:
                text = "Hinzufügen"
                break;
            case 1:
                text = "Woche zurücksetzen"
                break;
            case 2: // Hinzufügen oder Bearbeiten
            case 3:
            case 4:
            case 5:
                text = "Zurück"
                break;
            case 7:
                text = "Rückgängig"
                break;
            case 8:
                text = "Listenansicht"
                break;
            }
        }

        onClicked: {
            navMasterButton.forceActiveFocus()
            switch (state)
            {
            case 0:     // In Meal add
                mealAdd.visible = true
                state = 2
                break;
            case 1:
                messageBoxResetWeek.show()
                break
            case 2:
                if (!goBackWithOutSave.visible) {
                    goBackWithOutSave.show()
                    state = 9
                    return
                }
                break;
            case 3:
                overview.visible = false
                state = 0
                break;
            case 4:
                masterView.interactive = true
                weekView.hideWeekMealOverview()
                state = 1
                break;
            case 5:
                masterView.interactive = true
                weekView.hideMealSelectList()
                weekView.newListIndex = -1
                weekView.newSwipeIndex = -1

                state = 1
                break;
            case 6:
                qmlShoppingList.showAdd()

                break
            case 7:
                qmlShoppingList.undo()
                state = 6
                break
            case 8:
                homepage.visible = true
                state = 0
                break
            case 9:
                overview.visible = false
                mealEdit.visible = false
                mealAdd.visible = false

                state = 0
                break
            }
        }
    }
    Rectangle {
        anchors.margins: -5
        anchors.fill: navMasterButton
        color: primaryColor
        z: 1
    }

     MealOverview {
        id: overview
        z: 1
        visible: false
        anchors.fill: parent
        anchors.bottomMargin: navMasterButton.height
    }

    MealAdd {
        id: mealAdd
        z: 1
        anchors.fill: parent
        visible: false
        anchors.bottomMargin: 60
    }

    MealAdd {
        id: mealEdit
        visible: false
        z: 1
        anchors.fill: parent
        anchors.bottomMargin: 60
        onVisibleChanged: {
            if (!visible) {
                object.savePicture("")
            }
        }
    }

    SwipeView {
        id: masterView
        objectName: "masterView"
        anchors.fill: parent
        anchors.bottomMargin: 60
        currentIndex: 1
        spacing: 10

        MealRecommendation {
            id: mealAdvice
        }

        Rectangle {
            id: homepageContainer

            HomePage {
                id: homepage
                anchors.fill: parent
                z: 10
                onVisibleChanged: {
                    if (visible) {
                        if (swipeHomeObj != null)
                            swipeHomeObj.destroy()
                    }
                    else
                        createSwipeView()
                }
            }
        }

        WeekView {
            id: weekView
            objectName: "weekView"
            anchors.bottomMargin: -60
        }

        QmlShoppingList {
            id: qmlShoppingList
        }
        onCurrentIndexChanged: {
            switch(currentIndex)
            {
            case 0:
                break;
            case 1:
                if (homepage.visible)
                    navMasterButton.state = 0
                else
                    navMasterButton.state = 8
                break
            case 2:
                navMasterButton.state = 1
                break
            case 3:
                navMasterButton.state = 6
                break
            }
        }
    }
}
