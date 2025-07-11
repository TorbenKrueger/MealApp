#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QObject>

#include <QPair>

class MealItem;
class MealList;
class WeekViewItem;
class WeekView : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QList<WeekViewItem*>> list READ list NOTIFY listChanged)

public:
    explicit WeekView(MealList &mealList, QObject *parent = nullptr);
    ~WeekView();

    Q_INVOKABLE QString getMeal(int indexDay, int indexTime) const;
    Q_INVOKABLE void setMeal(int indexDay, int indexTime, MealItem *item);
    Q_INVOKABLE void setMeal(int indexDay, int indexTime, const QString &name, int persCount);

    Q_INVOKABLE int getPersCount(int indexDay, int indexTime) const;
    Q_INVOKABLE void setPersCount(int indexDay, int indexTime, int count);

    Q_INVOKABLE void removeMeal(const QString &uuid);
    Q_INVOKABLE void removeMeal(int indexDay, int indexTime);

    QList<QList<WeekViewItem*>> list() const;

    void setJson(const QJsonObject &json);
    QJsonObject getJson();

    MealItem *getMealItem(int index1, int index2);

signals:
    void saveWeekView();
    void listChanged();
    void mealChanged(int index1, int index2);

private slots:
    void saveAll();
    void itemDeleted(MealItem *item);
private:
    MealList *mMealList = nullptr;
    QList<QList<WeekViewItem*>> mQmlItems;

};

#endif // WEEKVIEW_H
