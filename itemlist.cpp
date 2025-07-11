#include "itemlist.h"

ItemList::ItemList(QObject *parent) : QObject(parent)
{

}

QVector<Item> ItemList::items() const
{
    return mItems;
}

bool ItemList::setItemAt(int index, const Item &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const Item &oldItem = mItems.at(index);

    if (oldItem.checked == item.checked &&
            oldItem.quantity == item.quantity &&
            oldItem.unit == item.unit &&
            oldItem.name == item.name) {
        return false;
    }

    mItems[index] = item;
    return true;
}

bool ItemList::appendItem(bool checked, double quantity, const QString &unit, const QString &name)
{
    for (const auto &tmp : mItems)
        if (tmp.name == name)
            return false;

    emit preItemAppended();

    Item item;
    item.checked = checked;
    item.quantity = quantity;
    item.unit = unit;
    item.name = name;

    mItems.append(item);

    emit postItemAppended();

    return true;
}

void ItemList::removeAllItems()
{
    while (mItems.size()) {
        emit preItemRemoved(0);
        mItems.remove(0);
        emit postItemRemoved();
    }
}

void ItemList::removeItem(int index)
{
    emit preItemRemoved(index);
    mItems.remove(index);
    emit postItemRemoved();
}
