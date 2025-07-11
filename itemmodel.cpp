#include "itemmodel.h"

#include "itemlist.h"

ItemModel::ItemModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const Item item = mList->items().at(index.row());
    switch (role) {
    case checkedRole:
        return QVariant(item.checked);
    case quantityRole:
        return QVariant(item.quantity);
    case unitRole:
        return QVariant(item.unit);
    case nameRole:
        return QVariant(item.name);
    }

    return QVariant();
}

bool ItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    Item item = mList->items().at(index.row());
    switch (role) {
    case checkedRole:
        item.checked = value.toBool();
        break;
    case quantityRole:
        item.quantity = value.toDouble();
        break;
    case unitRole:
        item.unit = value.toString();
        break;
    case nameRole:
        item.name = value.toString();
        break;
    }

    if (mList->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ItemModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[checkedRole] = "checked";
    names[quantityRole] = "quantity";
    names[unitRole] = "unit";
    names[nameRole] = "name";
    return names;
}

ItemList *ItemModel::list() const
{
    return mList;
}

void ItemModel::setList(ItemList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &ItemList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &ItemList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &ItemList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &ItemList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });
    }

    endResetModel();
}

QString ItemModel::getAllItems()
{
    QString master;
    for (const auto &item : mList->items()) {
        QString tmp = "%1 %2 %3\n";
        tmp = tmp.arg(QString::number(item.quantity), item.unit, item.name);
        master += tmp;
    }
    master.remove(master.length() - 1, 1);
    return master;
}
