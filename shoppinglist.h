#ifndef SHOPPINGLIST_H
#define SHOPPINGLIST_H

#include <QObject>
#include <QVector>
#include <QDate>
#include "unit.h"
#include <QMap>

#include "weekview.h"
#include "mealitem.h"

struct ShoppingItem
{
    bool buyed = false;
    double quantity = 0.0;
    Unit::Units unit = Unit::Units::noUnitRole;
    QString name;
    QDate addedAt;
    QDate buyedAt;
    QString uuid;
    QString mealUuid;
};

struct VisiShoppingItem
{
    QString groupUuid;
    QString name;
    QString unit;
    double quantity;
};

struct ShoppingItemGroup
{
    QString uuid;
    QString name;
    QList<ShoppingItem> items;

    bool add(ShoppingItem item)
    {
//        if (items.contains(item))
//            return false;

        items.append(item);
        return true;
    }

    bool remove(const QString &uuid)
    {
        for (int i = 0; i < items.count(); i++) {
            if (items.at(i).uuid == uuid) {
                items.removeAt(i);
                return true;
            }
        }
        return false;
    }

    double sum() const
    {
        if (items.length() <= 0)
            return 0;

        Unit::Units masterUnit = items.first().unit;
        double sum = 0;
        for (const auto &item : items)
        {
            double val = item.quantity;
            if (item.unit != masterUnit) {
                val = Unit::convertUnit(item.unit, val, masterUnit);
            }
            sum += val;
        }

        if ((masterUnit == Unit::Units::grammRole || masterUnit == Unit::Units::miliLitreRole) && sum >= 1000) {
            sum = Unit::convertUnit(masterUnit, sum, masterUnit == Unit::Units::grammRole ? Unit::Units::kgRole : Unit::Units::litreRole);

        }
        return sum;
    };

    QString unit() const
    {
        if (items.length() <= 0)
            return 0;

        Unit::Units masterUnit = items.first().unit;
        double sum = 0;
        for (const auto &item : items)
        {
            double val = item.quantity;
            if (item.unit != masterUnit) {
                val = Unit::convertUnit(item.unit, val, masterUnit);
            }
            sum += val;
        }

        if ((masterUnit == Unit::Units::grammRole || masterUnit == Unit::Units::miliLitreRole) && sum >= 1000) {
            if (masterUnit == Unit::Units::grammRole)
                masterUnit = Unit::Units::kgRole;
            else
                masterUnit = Unit::Units::litreRole;

        }
        return Unit::unitToString(masterUnit);
    };
};

class WeekView;
class MealList;
class ShoppingList : public QObject
{
    Q_OBJECT
public:
    explicit ShoppingList(MealList &list, WeekView &weekView, QObject *parent = nullptr);
    ~ShoppingList();

    QVector<VisiShoppingItem> items() const;

    bool setItemAt(int index, const VisiShoppingItem &item);

    void setWeekView(WeekView *view);
    void setItemList(QMap<QString, MealItem*> list);
signals:
    void dataChanged(int index);

    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void removedItem(const QString &text);

    void error(QString text);

public slots:
    void appendItem(ShoppingItem item);
    Q_INVOKABLE void appendItem(const QString &text);
    void appendItem(bool checked, double quantity, const QString &unit, const QString &name, const QString &mealUuid = "-1", const QDate &addedAt = QDate::currentDate(), const QDate &buyedAt = QDate::currentDate());

    void appendItem(VisiShoppingItem item);

    void insertItem(int index, VisiShoppingItem item);
    void removeAllItems();
    void removeItem(int index, bool saveLast = true);
    void undoLastItem();

private slots:
    void mealPropertyChanged(const QString &uuid);
    void mealListChanged(int index1, int index2);
private:
    MealList *mMealList = nullptr;
    WeekView *mWeekView = nullptr;


    QMap<QString, MealItem*> mMealItems;
    QVector<ShoppingItem> mAllItems;
    QVector<VisiShoppingItem> mVisiItems;
    QPair<int, VisiShoppingItem> mLastRemovedItem;
    QMap<QString, ShoppingItemGroup> mGroup;

    void save();
    bool getVisiItem(const QString &uuid);
    void load();
    void weekViewUpdated();
};

#endif // SHOPPINGLIST_H
