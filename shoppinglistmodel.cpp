#include "shoppinglistmodel.h"

#include <QDate>

#include "shoppinglist.h"
#include "unit.h"

#include <QDebug>

ShoppingListModel::ShoppingListModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{

}

int ShoppingListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant ShoppingListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const auto item = mList->items().at(index.row());
    switch (role) {
    case quantityRole:
        return item.quantity;
        break;
    case unitRole:
        return item.unit;
        break;
    case nameRole:
        QString name = item.name;
        if (name.isEmpty())
            return name;

        if (name.split(" ").length() == 2) {
            const auto l = name.split(" ");
            const auto letter = l.at(1).at(0).toUpper();
            name = l.at(1);
            name = l.at(0).toLower() + " " +name.replace(0, 1, letter);
            return name;
        }

        const auto letter = name.at(0).toUpper();
        name.replace(0, 1, letter);
        return name;
    }

    return QVariant();
}

bool ShoppingListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    auto item = mList->items().at(index.row());
    switch (role) {
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

Qt::ItemFlags ShoppingListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ShoppingListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[buyedRole] = "buyed";
    names[quantityRole] = "quantity";
    names[unitRole] = "unit";
    names[nameRole] = "name";
    names[addedAtRole] = "addedAt";
    names[buyedAtRole] = "buyedAt";
    return names;
}

ShoppingList *ShoppingListModel::list() const
{
    return mList;
}

void ShoppingListModel::setList(ShoppingList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &ShoppingList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &ShoppingList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &ShoppingList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &ShoppingList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });

        connect(mList, &ShoppingList::dataChanged, this, [=](int index) {
            dataChanged(this->createIndex(index, 0), this->createIndex(index, 0));

        });
    }

    endResetModel();
}


