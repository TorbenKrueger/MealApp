#include "categoryitem.h"

CategoryItem::CategoryItem(const QString &name, QObject *parent) : QObject(parent)
{
    mName = name;
}

void CategoryItem::setJson(const QJsonObject &json)
{

}

QJsonObject CategoryItem::getJson()
{

}

QString CategoryItem::name()
{
    return mName;
}

void CategoryItem::setIsBeef(bool isBeef)
{
    if (isBeef) {
        mIsVegan = false;
    }
    mIsBeef = isBeef;
}

void CategoryItem::setIsVegan(bool isVegan)
{
    if (isVegan) {
        mIsBeef = false;
    }
    mIsVegan = isVegan;
}

bool CategoryItem::isBeef() const
{
    return mIsBeef;
}

bool CategoryItem::isVegetable() const
{
    return !isBeef();
}

bool CategoryItem::isVegan() const
{
    return mIsVegan;
}
