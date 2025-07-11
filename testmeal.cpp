#include "testmeal.h"

TestMeal::TestMeal(QObject *parent) : QObject(parent)
{
    mMealList = new MealList(this);
}

QList<MealItem*> TestMeal::meals()
{
    QList<MealItem *> list;
    for (int i = 0; i < 10; i++) {
        MealItem *item = new MealItem();
        item->setName("Test Meal Item" + QString::number(i));
        item->setUuid("Test111" + QString::number(i));
        list.append(item);
    }
    return list;
}

MealList* TestMeal::list()
{
    return mMealList;
}

void TestMeal::setList(MealList *list)
{
    mMealList = list;
}
