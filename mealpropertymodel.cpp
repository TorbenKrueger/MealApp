#include "mealpropertymodel.h"

#include <QDebug>

#include "unit.h"
#include "mealproperty.h"

MealPropertyModel::MealPropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
}

int MealPropertyModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant MealPropertyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const auto item = mList->items().at(index.row());
    switch (role) {
    case uuidRole:
        return QVariant(item->uuid());
    case quantityRole:
        return QVariant(item->count());
    case unitRole:
        return QVariant(Unit::unitToString(Unit::stringToUnit(item->unit())));
    case nameRole:
        return QVariant(item->name());
    }

    return QVariant();
}

bool MealPropertyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
        return false;

    auto item = mList->items().at(index.row());

    if (!item)
        return false;

    switch (role) {
    case uuidRole:
        qDebug() << "Es wird nicht empfohlen die UUID der PRoperttys zu ändern !!!";
        item->setUuid(value.toString());
        break;
    case quantityRole:
        item->setCount(value.toDouble());
        break;
    case unitRole:
        item->setUnit(value.toString());
        break;
    case nameRole:
        item->setName(value.toString());
        break;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags MealPropertyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> MealPropertyModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[uuidRole] = "uuid";
    names[quantityRole] = "quantity";
    names[unitRole] = "unit";
    names[nameRole] = "name";
    return names;
}

MealProperties *MealPropertyModel::list() const
{
    return mList;
}

void MealPropertyModel::setList(MealProperties *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &MealProperties::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &MealProperties::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &MealProperties::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &MealProperties::postItemRemoved, this, [=]() {
            endRemoveRows();
        });

        connect(mList, &MealProperties::dataChanged, this, [=](int index) {
            dataChanged(this->createIndex(index, 0), this->createIndex(index, 0));

        });
    }

    endResetModel();
}

QString MealPropertyModel::getAllItems()
{
    QString master;
    for (const auto &item : mList->items()) {
        QString tmp = "%1 %2 %3\n";
        tmp = tmp.arg(QString::number(item->count()), item->unit(), item->name());
        master += tmp;
    }
    master.remove(master.length() - 1, 1);
    return master;
}
