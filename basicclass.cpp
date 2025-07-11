#include "basicclass.h"

#include <QRandomGenerator64>

#include <QWindow>
#include <QGuiApplication>

#include <QDebug>
#include <QDateTime>

BasicClass::BasicClass(QObject *parent)
{
    setParent(parent);
}

BasicClass::~BasicClass()
{
}

QString BasicClass::dateFormat()
{
    return "dd.MM.yyyy";
}

QString BasicClass::umlauts(QString text, bool aeToA)
{
    if (aeToA) {
        text.replace("umlaut_ae", "ä");
        text.replace("umlaut_oe", "ö");
        text.replace("umlaut_ue", "ü");

        text.replace("umlaut_AE", "Ä");
        text.replace("umlaut_OE", "Ö");
        text.replace("umlaut_UE", "Ü");

        text.replace("umlaut_Ae", "Ä");
        text.replace("umlaut_Oe", "Ö");
        text.replace("umlaut_Ue", "Ü");

        text.replace("umlaut_ss", "ß");
    }
    else {
        text.replace("ß", "umlaut_ss");

        text.replace("ä", "umlaut_ae");
        text.replace("ö", "umlaut_oe");
        text.replace("ü", "umlaut_ue");

        text.replace("Ä", "umlaut_AE");
        text.replace("Ö", "umlaut_OE");
        text.replace("Ü", "umlaut_UE");
    }
    return text;
}

bool BasicClass::eventFilter(QObject *obj, QEvent *evt)
{
    return false;
}

void BasicClass::simulateTouch()
{
//    qDebug() << "Send Event";
//    QTouchDevice *screen = new QTouchDevice();
//    QList<QTouchEvent::TouchPoint> points;
//    QTouchEvent::TouchPoint p;
//    p.setPos(QPointF(0,0));
//    p.setId(2000001);
//    p.setPressure(0.5);
//    points.append(p);
//    QTouchEvent *tEvent1 = new QTouchEvent(QEvent::TouchBegin, screen, Qt::NoModifier, Qt::TouchPointPressed, points);


//    const auto win = QGuiApplication::topLevelWindows().first();
//qDebug () << win;

//    const auto obj = static_cast<QObject*>(win);
//qDebug() << obj;


//    QGuiApplication::sendEvent(obj, tEvent1);

//    qDebug() << "Send Event Ende";
}

QString BasicClass::createUUID(const QString &name)
{
    QString uuid;
    const auto time = QDateTime::currentDateTime().toSecsSinceEpoch();

    const int max = 999999;
    const int first = QRandomGenerator::global()->bounded(max);
    const int second = QRandomGenerator::global()->bounded(max);

    uuid = QString::number(time) + ":" + QString::number(first)+ "_" + QString::number(second) + ":" + QString::number(QTime::currentTime().msec()) + ":" + umlauts(name, false);

    return uuid;
}
