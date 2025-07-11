#include "meallistmodel.h"

#include <QDebug>

#include "meallist.h"
#include "mealitem.h"

MealListModel::MealListModel(QObject *parent)
    : QAbstractListModel(parent)
  , mList(nullptr)
{
}

int MealListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant MealListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    auto item = mList->items().at(index.row());
    switch (role) {
    case uuidRole:
        return item->uuid();
    case nameRole:
        return item->name();
    case ratingRole:
        return item->rating();
    case picRole:
        return item->picturePath();
    case picRotaionRole:
        return item->rotation();
    case objectRole:
        return QVariant::fromValue<MealItem*>(item);
    }

    return QVariant();
}

bool MealListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    auto item = mList->items().at(index.row());

    if (!item)
        return false;

    switch (role) {
    case uuidRole:
        qDebug() << "Die UUID sollte nicht geändert werden!!!";
        item->setUuid(value.toString());
        break;
    case nameRole:
        item->setName(value.toString());
        break;
    case ratingRole:
        item->setRating(value.toDouble());
        break;
    case picRole:
        item->setPicturePath(value.toString());
        break;
    case picRotaionRole:
        item->setRotation(value.toInt());
        break;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags MealListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> MealListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[uuidRole] = "uuid";
    names[nameRole] = "name";
    names[ratingRole] = "rating";
    names[picRole] = "picturePath";
    names[picRotaionRole] = "pictureRotation";
    names[objectRole] = "object";
    return names;
}

MealList *MealListModel::list() const
{
    return mList;
}

void MealListModel::setList(MealList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &MealList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &MealList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &MealList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &MealList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });

        connect(mList, &MealList::dataChanged, this, [=](int index) {
            dataChanged(this->createIndex(index, 0), this->createIndex(index, 0));
        });

        //connect(mList, &MealList::orderBy, this, &MealListModel::sortMeals);
    }

    endResetModel();
}
