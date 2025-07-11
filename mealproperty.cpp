#include "mealproperty.h"

#include "basicclass.h"

MealProperties::MealProperties(QObject *parent) : QObject(parent)
{

}

QList<MealProperty *> MealProperties::property() const
{
    return mProperties;
}
QList<MealProperty *> MealProperties::items() const
{
    return property();
}

bool MealProperties::addProperty(const QString &uuid, double count, const QString &unit, const QString &name)
{
    for (auto &item : mProperties)
        if (item->name().toLower() == name.toLower())
            return false;

    emit preItemAppended();

    MealProperty *prop = new MealProperty(this, uuid);
    prop->setCount(count);
    prop->setUnit(unit);
    prop->setName(name);
    mProperties.append(prop);

    connect(prop, &MealProperty::dataChanged, this, [=]()
    {
        emit dataChanged(indexOfItem(prop));
    });

    emit postItemAppended();
    emit dataChanged(indexOfItem(prop));

    return true;
}

int MealProperties::indexOfItem(MealProperty *item)
{
    for (int i = 0; i < mProperties.length(); i++)
        if (mProperties.at(i)->uuid() == item->uuid())
            return i;

    return -1;
}

void MealProperties::removeProperty(const QString &uuid)
{
    auto item = uuidToItem(uuid);
    if (!item)
        return;

    int index = 0;
    for (index = 0; index < mProperties.length(); index++)
        if (mProperties.at(index)->uuid() == uuid)
            break;

    emit preItemRemoved(index);
    mProperties.removeAt(index);
    emit postItemRemoved();

    emit dataChanged(-1);

    delete item;
}

void MealProperties::removeAll()
{
    for (auto &obj : mProperties)
        removeProperty(obj->uuid());
}

MealProperty* MealProperties::uuidToItem(const QString &uuid)
{
    for (auto &obj : mProperties)
        if (obj->uuid() == uuid)
            return obj;

    return nullptr;
}

//------------------------------------------------------------------------

MealProperty::MealProperty(QObject *parent, const QString &uuid) : QObject(parent)
{
    setUuid(uuid);
}


QString MealProperty::uuid() const
{
    return mUuid;
}
void MealProperty::setUuid(const QString &uuid)
{
    mUuid = uuid;
    emit uuidChanged(uuid);
    emit dataChanged();
}

double MealProperty::count() const
{
    return mCount;
}
void MealProperty::setCount(double count)
{
    mCount = count;
    emit countChanged(count);
    emit dataChanged();
}

QString MealProperty::unit() const
{
    return mUnit;
}
void MealProperty::setUnit(const QString &unit)
{
    mUnit = unit;
    emit unitChanged(unit);
    emit dataChanged();
}

QString MealProperty::name() const
{
    return mName;
}
void MealProperty::setName(const QString &name)
{
    mName = name;
    emit nameChanged(name);
    emit dataChanged();
}
