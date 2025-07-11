import QtQuick 2.15

import "../basic"

Rectangle {
    id: root
    height: star1.height
    width: star1.width * 5
    color: "transparent"

    property color defColor: "gray"
    property color selColor: "gold"

    property double size: 0.3
    property double stars: 0 // rating

    onStarsChanged: {
        setRating(stars)
    }

    signal ratingChanged(var rating)

    function update(){setRating(stars);}

    function setRating(rating)
    {
        star1.starColor = defColor
        star2.starColor = defColor
        star3.starColor = defColor
        star4.starColor = defColor
        star5.starColor = defColor

        if (rating >= 1)
            star1.starColor = selColor
        if (rating >= 2)
            star2.starColor = selColor
        if (rating >= 3)
            star3.starColor = selColor
        if (rating >= 4)
            star4.starColor = selColor
        if (rating >= 5)
            star5.starColor = selColor
    }

    Row {
        QmlStar {
            id: star1
            starColor: defColor
            size: root.size
            onStarClicked: {
                if (star1.starColor == selColor && star2.starColor == defColor) {
                    star1.starColor = defColor
                    stars = 0
                }
                else {
                    star1.starColor = selColor
                    stars = 1
                }

                star2.starColor = defColor
                star3.starColor = defColor
                star4.starColor = defColor
                star5.starColor = defColor

                ratingChanged(stars)
            }
        }
        QmlStar {
            id: star2
            size: root.size
            starColor: defColor
            onStarClicked: {
                star1.starColor = selColor
                star2.starColor = selColor
                star3.starColor = defColor
                star4.starColor = defColor
                star5.starColor = defColor
                stars = 2

                ratingChanged(stars)
            }
        }
        QmlStar {
            id: star3
            size: root.size
            starColor: defColor
            onStarClicked: {
                star1.starColor = selColor
                star2.starColor = selColor
                star3.starColor = selColor
                star4.starColor = defColor
                star5.starColor = defColor
                stars = 3

                ratingChanged(stars)
            }
        }
        QmlStar {
            id: star4
            size: root.size
            starColor: defColor
            onStarClicked: {
                star1.starColor = selColor
                star2.starColor = selColor
                star3.starColor = selColor
                star4.starColor = selColor
                star5.starColor = defColor
                stars = 4

                ratingChanged(stars)
            }
        }
        QmlStar {
            id: star5
            size: root.size
            starColor: defColor
            onStarClicked: {
                star1.starColor = selColor
                star2.starColor = selColor
                star3.starColor = selColor
                star4.starColor = selColor
                star5.starColor = selColor
                stars = 5

                ratingChanged(stars)
            }
        }
    }
}
