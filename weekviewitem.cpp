#include "weekviewitem.h"

#include "mealitem.h"

#include <QDebug>

#include "weekview.h"

WeekViewItem::WeekViewItem(MealItem *item, QObject *parent) : QObject(parent)
{
    if (parent == nullptr)
        setParent(item);

    setMealItem(item);
}

void WeekViewItem::setMealItem(MealItem *item, int persCount)
{
    mItem = item;
    mPersonCount = persCount;

    if (persCount == -1 && item)
        mPersonCount = item->persCount();

    mValid = item;

    emit validChanged(mValid);
    emit objectChanged();
    emit dataChanged();

    if (item) {
        connect(mItem, &MealItem::dataChanged, this, &WeekViewItem::objectChanged);
        connect(mItem, &MealItem::dataChanged, this, &WeekViewItem::dataChanged);
        connect(mItem, &MealItem::toRemove, this, &WeekViewItem::itemDeleted, Qt::DirectConnection);
    }
}

bool WeekViewItem::isValid()
{
    return mValid;
}

QString WeekViewItem::uuid() const
{
    if (mItem)
        return mItem->uuid();

    return "";
}
void WeekViewItem::setUuid(const QString &uuid)
{
    qDebug() << "Die UUID sollte nicht  hier gesetzt werden!!!";
    return;
    //    if (mValid) {
    //        mUuid = uuid;
    //        emit uuidChanged(uuid);
    //        emit dataChanged();
    //    }
}

QString WeekViewItem::name() const
{
    if (mItem)
        return mItem->name();

    return "";
}
void WeekViewItem::setName(const QString &name)
{
    if (mItem) {
        mItem->setName(name);

        emit nameChanged(name);
        emit dataChanged();
    }
}

QString WeekViewItem::picPath() const
{
    if (mItem)
        return mItem->picturePath();

    return "";
}
void WeekViewItem::setPicPath(const QString &path)
{
    if (mItem) {
        mItem->setPicturePath(path);

        emit picPathChanged(path);
        emit dataChanged();
    }
}

double WeekViewItem::rating() const
{
    if (mItem)
        return mItem->rating();

    return 0;
}
void WeekViewItem::setRating(double rating)
{
    if (mItem) {
        mItem->setRating(rating);
        emit ratingChanged(rating);
        emit dataChanged();
    }
}

int WeekViewItem::persCount() const
{
    return mPersonCount;
}
void WeekViewItem::setPersCount(int count)
{
    if (mPersonCount != count) {
        mPersonCount = count;
        emit personCountChanged(count);
        emit dataChanged();
    }
}

int WeekViewItem::picRotation() const
{
    if (mItem)
        return mItem->rotation();

    return 0;
}
void WeekViewItem::setPicRotation(int rotation)
{
    if (mItem) {
        mItem->setRotation(rotation);

        emit picRotationChanged(rotation);
        emit dataChanged();
    }
}

MealItem *WeekViewItem::object()
{
    if (!mItem && false)
        qDebug() << "!!! WARNING !!! <WeekViewItem::object() returned nullptr> <"<<sender()<<">";

    return mItem;
}
