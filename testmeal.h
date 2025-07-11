#ifndef TESTMEAL_H
#define TESTMEAL_H

#include <QObject>

#include "mealitem.h"

#include "meallist.h"

class TestMeal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<MealItem *> meals READ meals)
    Q_PROPERTY(MealList* list READ list WRITE setList)
public:
    explicit TestMeal(QObject *parent = nullptr);
    QList<MealItem *> meals();

    void setList(MealList *list);
    MealList *list();
signals:

private:
    MealList *mMealList = nullptr;

};

#endif // TESTMEAL_H
