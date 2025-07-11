#include "itemclass.h"

ItemClass::ItemClass(QObject *parent) : QObject(parent)
{
    Item item;
    item.isChecked = false;
    item.quantity = 0;
    item.unit = "noUnit";
    item.name = "Test1";
    mItems.append(item);

    Item item2;
    item2.isChecked = false;
    item2.quantity = 0;
    item2.unit = "noUnit";
    item2.name = "Test2";
    mItems.append(item2);
}

QVector<Item> ItemClass::items() const
{
    return mItems;
}

bool ItemClass::setItemAt(int index, const Item &item)
{
    if (index < 0 || index > mItems.size())
        return false;

    mItems[index] = item;
    return true;
}

void ItemClass::appendItem()
{
    emit preItemAppended();

    Item item;
    item.isChecked = false;
    item.quantity = 0;
    item.unit = "noUnit";
    item.name = "noName";

    mItems.append(item);

    emit postItemAppended();
}

void ItemClass::removeItem(int index)
{
    emit preItemRemoved(index);

    mItems.remove(index);

    emit postItemRemoved();
}

