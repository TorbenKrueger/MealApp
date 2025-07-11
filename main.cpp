#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include <QQmlComponent>

#include "shoppinglistmodel.h"
#include "shoppinglist.h"

#include "weekview.h"
#include "mealitem.h"
#include "mealproperty.h"

#include "meallist.h"
#include "meallistmodel.h"
#include "mealpropertymodel.h"

#include <QDebug>

#include <QJsonArray>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    app.setOrganizationName("krueger");
    app.setOrganizationDomain("");

    qmlRegisterType<ShoppingListModel>("CModel", 1, 0, "ShoppingListModel");
    qmlRegisterUncreatableType<ShoppingList>("CModel", 1, 0, "ShoppingList",
                                         QStringLiteral("ShoppingList should not be created in QML"));

    qmlRegisterType<MealListModel>("CModel", 1, 0, "MealListModel");
    qmlRegisterUncreatableType<MealList>("CModel", 1, 0, "MealList",
                                         QStringLiteral("MealList should not be created in QML"));

    qmlRegisterType<MealPropertyModel>("CModel", 1, 0, "MealPropertyModel");
    qmlRegisterUncreatableType<MealProperties>("CModel", 1, 0, "MealProperties",
                                         QStringLiteral("MealProperties should not be created in QML"));

    qRegisterMetaType<QList<QList<MealItem*>>>("QList<QList<WeekViewItem*> >");

    //1
    MealList mealList;  // Alle Gerichte werden geladen
    engine.rootContext()->setContextProperty(QStringLiteral("mealList"), &mealList);
    //2
    MealProperties mealProperties; // Properties des gerichte
    engine.rootContext()->setContextProperty(QStringLiteral("mealProperties"), &mealProperties);
    //3
    WeekView weekView(mealList);    // Wochenübersicht wird geladen / MealList muss exestieren
    engine.rootContext()->setContextProperty("MealWeekView", &weekView);
    //4
    ShoppingList shoppingList(mealList, weekView);  // Alle Zutaten (keine Gerichte) werden geladen
    engine.rootContext()->setContextProperty(QStringLiteral("shoppingList"), &shoppingList);

    const QUrl url(QStringLiteral("qrc:/qml/main/main.qml"));
    QQmlComponent component(&engine, url);
    QObject *object = component.create();
    qDebug() << "FEHLER" << component.errorString();

    QObject::connect(&mealList, &MealList::error, object, [=](const QString &error) {
        QMetaObject::invokeMethod(object, "setError", Q_ARG(QVariant, error));
    });
    QObject::connect(&mealList, &MealList::close, object, [=]() {
        qApp->quit();
    });
    QObject::connect(&mealList, &MealList::newMealFromServer, object, [=](QString uuid, QString name, QString date) {
        qDebug() << "MAIN nächstes essen";
        QMetaObject::invokeMethod(object, "newMealFromServer", Q_ARG(QVariant, uuid), Q_ARG(QVariant, name), Q_ARG(QVariant, date));
    });
    QObject::connect(&mealList, &MealList::newMealFromServer, object, [=](QString uuid, QString name, QString date) {
        qDebug() << "MAIN nächstes essen";
        QMetaObject::invokeMethod(object, "newMealFromServer", Q_ARG(QVariant, uuid), Q_ARG(QVariant, name), Q_ARG(QVariant, date));
    });
    QObject::connect(&mealList, &MealList::loadedBackup, object, [=](QJsonArray ary) {
        qDebug() << "MAIN Backup geladen";
        QMetaObject::invokeMethod(object, "showFeedbackLabel", Q_ARG(QVariant, "Backup vom Server geladen"), Q_ARG(QVariant, 5000));
    });

    return app.exec();
}
