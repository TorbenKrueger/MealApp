import QtQuick 2.0
import QtQuick.Controls 2.0

import CModel 1.0

Rectangle {
    id: homepageRect
    color: primaryColor

    SwipeView {
        anchors.fill: parent
        anchors.margins: 5
        anchors.bottomMargin: 10
        spacing: 10
        Repeater {
            model: mealList.count
            Loader {
                active: true // || SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                sourceComponent: MealOverview {
                    visible: true
                    object: mealList.item(index)
                    showToogleViewAction: true
                }
            }
        }
    }
}
