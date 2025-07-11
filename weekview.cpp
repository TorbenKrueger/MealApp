#include "weekview.h"
#include <QQuickView>
#include <QQmlEngine>
#include <QQuickItem>
#include <QDebug>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QSettings>

#include "weekviewitem.h"
#include "meallist.h"

WeekView::WeekView(MealList &mealList, QObject *parent) : QObject(parent)
{
    mMealList = &mealList;

    for (int i = 0; i < 7; i++) {
        QList<WeekViewItem*> list;
        for (int k = 0; k < 3; k++) {
            const auto item = new WeekViewItem(nullptr, this);
            list.append(item);
        }
        mQmlItems.append(list);
    }

    QSettings settings("krueger", "mealApp");
    const auto tmp = settings.value("weekMeals").toString();
    const auto obj = QJsonDocument::fromJson(tmp.toLatin1()).object();

    setJson(obj);

    connect(this, &WeekView::saveWeekView, this, &WeekView::saveAll);
}

WeekView::~WeekView()
{
    saveAll();
}

void WeekView::saveAll()
{
    QSettings settings("krueger", "mealApp");
    QJsonObject jsonObj = getJson();

    QJsonDocument doc;
    doc.setObject(jsonObj);

    const auto data = doc.toJson();
    settings.setValue("weekMeals", data);

    emit listChanged();
}

MealItem* WeekView::getMealItem(int index1, int index2)
{
    if (mQmlItems[index1][index2])
        return mQmlItems[index1][index2]->object();

    return nullptr;
}

QString WeekView::getMeal(int indexDay, int indexTime) const
{
    const auto obj = mQmlItems[indexDay][indexTime];
    if (obj && obj->object())
        return obj->object()->name();

    return "";
}
void WeekView::setMeal(int indexDay, int indexTime, MealItem *item)
{
   setMeal(indexDay, indexTime, item->uuid(), -1);
}
void WeekView::setMeal(int indexDay, int indexTime, const QString &name, int persCount)
{
    const auto mealItem = mMealList->getItemByUuid(name);
    auto item = mQmlItems[indexDay][indexTime];

    connect(item, &WeekViewItem::itemDeleted, this, &WeekView::itemDeleted);
    connect(item, &WeekViewItem::dataChanged, this, &WeekView::listChanged);
    connect(item, &WeekViewItem::personCountChanged, this, &WeekView::saveWeekView);
    connect(item, &WeekViewItem::personCountChanged, this, [=]()
    {
        emit mealChanged(indexDay, indexTime);
    });

    int _persCount = persCount;
    if (persCount == -1)
        _persCount = item->persCount();

    item->setMealItem(mealItem, _persCount);

    if (item)
        emit mealChanged(indexDay, indexTime);

    emit saveWeekView();
}

int WeekView::getPersCount(int indexDay, int indexTime) const
{
    const auto obj = mQmlItems[indexDay][indexTime];
    if (obj)
        return obj->persCount();
    return 0;
}
void WeekView::setPersCount(int indexDay, int indexTime, int count)
{
    if (!mQmlItems[indexDay][indexTime] || mQmlItems[indexDay][indexTime]->persCount() == count)
        return;

    mQmlItems[indexDay][indexTime]->setPersCount(count);

    emit mealChanged(indexDay, indexTime);
    emit saveWeekView();
}

void WeekView::removeMeal(const QString &uuid)
{
    for (int i = 0; i < 7; i++) {
        QList<QPair<QString, int>> tmp;
        for (int k = 0; k < 3; k++) {
            if (mQmlItems[i][k] && mQmlItems[i][k]->uuid() == uuid) {
                removeMeal(i, k);
                return;
            }
        }
    }
}
void WeekView::removeMeal(int indexDay, int indexTime)
{
    auto item = mQmlItems[indexDay][indexTime];
    if (item && item->object())
        item->object()->setLastCooked(QDate::currentDate());
    mQmlItems[indexDay][indexTime]->setMealItem(nullptr);

    emit mealChanged(indexDay, indexTime);
    emit saveWeekView();
}

QJsonObject WeekView::getJson()
{
    QJsonObject json;

    for (int i = 0; i < mQmlItems.length(); i++) {
        const auto list = mQmlItems[i];

        QJsonObject times;

        for (int k = 0; k < list.length(); k++) {
            if (!list.value(k))
                continue;

            if (!list.value(k)->object())
                continue;

            const auto mealName = list.value(k)->object()->uuid();
            const auto persCount = list.value(k)->persCount();

            QJsonArray array;
            array.append(mealName);
            array.append(persCount);

            times[QString::number(k)] = array;
        }
        json[QString::number(i)] = times;
    }

    return json;
}
void WeekView::setJson(const QJsonObject &json)
{
    const auto obj = json;
    const auto keys = obj.keys();

    for (int i = 0; i < keys.length(); i++) {
        const auto timesObj = obj[keys.at(i)].toObject();

        for (int k = 0; k < timesObj.keys().length(); k++) {
            const auto mealName = timesObj[timesObj.keys().at(k)].toArray().first().toString();
            const auto persCount = timesObj[timesObj.keys().at(k)].toArray().last().toInt();

            setMeal(keys.at(i).toInt(), timesObj.keys().at(k).toInt(), mealName, persCount);
        }
    }
}

void WeekView::itemDeleted(MealItem *item)
{
    if (item)
        removeMeal(item->uuid());
    else
        qDebug() << "!!! WARNING !!! <MealItem deleted before removed from WeekView>";
}

QList<QList<WeekViewItem*>> WeekView::list() const
{
    return mQmlItems;
}
