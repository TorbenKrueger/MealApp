#include "shoppinglist.h"

#include "unit.h"
#include "basicclass.h"

#include <QDebug>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "meallist.h"
#include "weekview.h"

ShoppingList::ShoppingList(MealList &list, WeekView &weekView, QObject *parent) : QObject(parent)
{
    mMealList = &list;
    mWeekView = &weekView;

    weekViewUpdated();

    connect(mMealList, &MealList::propertyChanged, this, &ShoppingList::mealPropertyChanged);
    connect(mWeekView, &WeekView::mealChanged, this, &ShoppingList::mealListChanged);

    load();
}

ShoppingList::~ShoppingList()
{
    save();
}

QVector<VisiShoppingItem> ShoppingList::items() const
{
    return mVisiItems;
}

bool ShoppingList::setItemAt(int index, const VisiShoppingItem &item)
{
    if (index < 0 || index >= mVisiItems.size())
        return false;

    const auto &oldItem = mVisiItems.at(index);

    if (oldItem.quantity == item.quantity &&
            oldItem.unit == item.unit &&
            oldItem.name == item.name) {
        return false;
    }

    mVisiItems[index] = item;
    emit dataChanged(index);
    return true;
}

void ShoppingList::appendItem(const QString &text)
{
    const auto list = text.split(QStringLiteral(" "));

    QStringList tmp;

    for (int i = 0; i < list.length(); i++) {
        if (list.at(i).isEmpty())
            continue;

        tmp.append(list.at(i));
    }

    bool ok;

    if (tmp.isEmpty())
        return;

    const int count = tmp.first().toInt(&ok);
    if (!ok)
        return;

    QString unit = "";
    QString name = "";

    if (tmp.length() == 2) {
        name = tmp.last();
    }
    else if(tmp.length() == 3) {
        unit = tmp.at(1);
        name = tmp.last();
    }
    else {
        return;
    }
    appendItem(false, count, unit, name, "-1"); // -1 -> vom User hinzugefügt

    save();
}
void ShoppingList::appendItem(bool buyed, double quantity, const QString &unit, const QString &name, const QString &mealUuid, const QDate &addedAt, const QDate &buyedAt)
{
    ShoppingItem item;
    item.buyed = buyed;
    item.quantity = quantity;
    item.unit = Unit::stringToUnit(unit);
    item.name = name;
    item.mealUuid = mealUuid;
    item.uuid = BasicClass::createUUID("shoppingItem_" + name.toLower());
    item.addedAt = addedAt;
    item.buyedAt = buyedAt;

    appendItem(item);
}
void ShoppingList::appendItem(ShoppingItem item)
{
    if (item.uuid.isEmpty())
        item.uuid = BasicClass::createUUID("shoppingItem_" + item.name.toLower());

    for (const auto &i : mAllItems){
        if (i.uuid == item.uuid) {
            emit error("Fehler bei der UUID -> App neustarten");
            return;
        }
    }

    mAllItems.append(item);

    const auto name = item.name.toLower();

    if (mGroup.contains(name.toLower())) {
        ShoppingItemGroup group = mGroup.value(name.toLower());
        const auto ret = group.add(item);
        if (!ret)
            qDebug () << "Item ist bereits in der Gruppe";
        else
            mGroup.insert(name.toLower(), group);
    }
    else {
        ShoppingItemGroup newGroup;
        newGroup.uuid = BasicClass::createUUID("shoppingItemGroup_" + name.toLower());
        newGroup.name = name.toLower();
        newGroup.add(item);
        mGroup.insert(name.toLower(), newGroup);
    }

    const auto update = getVisiItem(mGroup.value(name.toLower()).uuid);

    VisiShoppingItem visiItem;

    visiItem.groupUuid = mGroup.value(name.toLower()).uuid;
    visiItem.name = name;
    visiItem.quantity = mGroup.value(name.toLower()).sum();
    visiItem.unit = mGroup.value(name.toLower()).unit();

    if (update) {
        int index = -1;

        for (int i = 0; i < mVisiItems.length(); i++) {
            if (mVisiItems.at(i).groupUuid == visiItem.groupUuid) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            qDebug() << "Fehler beim setzten des Items";
            return;
        }

        setItemAt(index, visiItem);
    }
    else {
        appendItem(visiItem);
    }
}

void ShoppingList::appendItem(VisiShoppingItem item)
{
    emit preItemAppended();
    mVisiItems.append(item);
    emit postItemAppended();
}

bool ShoppingList::getVisiItem(const QString &uuid)
{
    for (const auto &visiItem : mVisiItems) {
        if (visiItem.groupUuid.toLower() == uuid.toLower())
            return true;
    }

    return false;
}

void ShoppingList::insertItem(int index, VisiShoppingItem item)
{
    qDebug() << "Diese Funktion funktioniert noch nicht";
    //    return;
    QList<VisiShoppingItem> list;

    for (int i = index; i < mVisiItems.count(); i++)
        list.append(mVisiItems.at(i));

    const int removeLen = mVisiItems.length() - list.length();
    while(mVisiItems.length() > removeLen)
        removeItem(mVisiItems.length() - 1);

    appendItem(item);

    for (int i = 0; i < list.count(); i++)
        appendItem(list.at(i));
}

void ShoppingList::removeAllItems()
{
    while (mVisiItems.size()) {
        emit preItemRemoved(0);
        mVisiItems.remove(0);
        emit postItemRemoved();
    }
}

void ShoppingList::removeItem(int index, bool saveLast)
{
    if (saveLast) {
        QPair<int, VisiShoppingItem> pair;
        pair.first = index;
        pair.second = mVisiItems.value(index);
        mLastRemovedItem = pair;
    }

    emit removedItem(mVisiItems.value(index).name);

    emit preItemRemoved(index);

    auto item = mVisiItems.value(index);
    auto group = mGroup.value(item.name.toLower());

    group.remove(item.groupUuid);

    if (group.items.isEmpty())
        mGroup.remove(group.name.toLower());

    mVisiItems.remove(index);

    QVector<ShoppingItem> tmpList;

    for (const auto &allItem : mAllItems) {
        if (allItem.name.toLower() == item.name.toLower())
            continue;
        tmpList.append(allItem);
    }

    mAllItems.clear();
    mAllItems = tmpList;

    emit postItemRemoved();

    save();
}

void ShoppingList::undoLastItem()
{
    if (mLastRemovedItem.first == -1)
        return;

    if (mLastRemovedItem.first >= mVisiItems.size()) {
        appendItem(mLastRemovedItem.second);
    }
    else {
        insertItem(mLastRemovedItem.first, mLastRemovedItem.second);
    }

    mLastRemovedItem.first = -1;

    save();
}

void ShoppingList::save()
{
    QSettings settings("krueger", "mealApp");
    settings.setValue("shoppingList", "");

    QJsonArray array;

    for (const auto &item : mAllItems) {
        if (item.mealUuid != "-1")
            continue;

        QJsonObject obj;
        obj["buyed"] = item.buyed;
        obj["quantity"] = item.quantity;
        obj["unit"] = Unit::unitToString(item.unit);
        obj["name"] = item.name;
        obj["addedat"] = item.addedAt.toString("dd.MM.yyyy");
        obj["buyedat"] = item.buyedAt.toString("dd.MM.yyyy");
        obj["uuid"] = item.uuid;
        obj["mealuuid"] = item.mealUuid;

        array.append(obj);
    }

    QJsonDocument doc;
    doc.setArray(array);

    const auto data = doc.toJson();
    settings.setValue("shoppingList", data);
}

void ShoppingList::load()
{
    QSettings settings("krueger", "mealApp");
    const auto data = settings.value("shoppingList").toByteArray();
    //    settings.setValue("shoppingList", "");
    //    return;

    QJsonDocument doc = QJsonDocument::fromJson(data);

    const auto array = doc.array();
    for (int i = 0; i < array.count(); i++) {
        const auto obj = array.at(i).toObject();

        ShoppingItem item;
        item.buyed = obj["buyed"].toBool();
        item.quantity = obj["quantity"].toDouble();
        item.unit = Unit::stringToUnit(obj["unit"].toString());
        item.name = obj["name"].toString();
        item.addedAt = QDate::fromString(obj["addedat"].toString(),"dd.MM.yyyy");
        item.buyedAt = QDate::fromString(obj["buyedat"].toString(),"dd.MM.yyyy");
        item.uuid = obj["uuid"].toString();
        item.mealUuid = obj["mealuuid"].toString();

        appendItem(item);
    }
}

void ShoppingList::setWeekView(WeekView *view)
{
    mWeekView = view;
}

void ShoppingList::setItemList(QMap<QString, MealItem *> list)
{
    mMealItems = list;
    weekViewUpdated();
}

void ShoppingList::weekViewUpdated()
{
    QMap<QString, ShoppingItemGroup> tmpGroup;
    for (const auto &group : mGroup) {
        QList<ShoppingItem> tmpItems;
        const auto groupUuid = group.uuid;
        for (const auto &item : group.items) {
            if (item.mealUuid != "-1") {
                const auto uuid = item.uuid;
                for (int i = 0; i < mAllItems.length(); i++) {
                    if (mAllItems[i].uuid == uuid) {
                        mAllItems.remove(i);
                        break;
                    }
                }
                continue;
            }
            tmpItems.append(item);
        }

        if (!tmpItems.isEmpty()) {
            ShoppingItemGroup newGroup;
            newGroup.uuid = group.uuid;
            newGroup.name = group.name;
            newGroup.items = tmpItems;
            tmpGroup.insert(group.name, newGroup);
        }
    }

    mGroup = tmpGroup;
    removeAllItems();

    // Items ohne Essen (Meal) hinzufügen
    for (const auto &group : mGroup)
    {
        VisiShoppingItem item;
        item.groupUuid = group.uuid;
        item.name = group.name;
        item.quantity = group.sum();
        item.unit = group.unit();

        appendItem(item);
    }

    // Items mit Essen (Meal) hinzufügen
    for (int i = 0; i < 7; i++) {
        for (int k = 0; k < 3; k++) {
            const auto item =  mWeekView->getMealItem(i, k);
            if (!item)
                continue;

            const int cnt = mWeekView->getPersCount(i, k);

            const auto uuid = item->uuid();
            for (const auto &prop : item->getProperties()) {
                if (prop.length() != 3)
                    continue;

                appendItem(false, prop[0].toDouble() * cnt, prop[1], prop[2], uuid);
            }
        }
    }
}

void ShoppingList::mealPropertyChanged(const QString &uuid)
{
    Q_UNUSED(uuid);

    weekViewUpdated();
}

void ShoppingList::mealListChanged(int index1, int index2)
{
    Q_UNUSED(index1);
    Q_UNUSED(index2);
    weekViewUpdated();
}
