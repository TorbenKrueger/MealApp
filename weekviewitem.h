#ifndef WEEKVIEWITEM_H
#define WEEKVIEWITEM_H

#include <QObject>

class MealItem;
class WeekViewItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(MealItem* object READ object NOTIFY objectChanged)

    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString picPath READ picPath WRITE setPicPath NOTIFY picPathChanged)
    Q_PROPERTY(double rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int personCount READ persCount WRITE setPersCount NOTIFY personCountChanged)
    Q_PROPERTY(int picRotation READ picRotation WRITE setPicRotation NOTIFY picRotationChanged)

public:
    explicit WeekViewItem(MealItem *item, QObject *parent = nullptr);

    int personCount() const;
    void setPersonCount(int count);

    QString uuid() const;
    void setUuid(const QString &uuid);

    QString name() const;
    void setName(const QString &name);

    QString picPath() const;
    void setPicPath(const QString &path);

    double rating() const;
    void setRating(double rating);

    int persCount() const;
    void setPersCount(int cnt);

    int picRotation() const;
    void setPicRotation(int rotation);

    MealItem* object();

    void setMealItem(MealItem *item, int persCount = -1);

    bool isValid();
signals:
    void dataChanged();

    void objectChanged();
    void itemDeleted(MealItem *item);

    void validChanged(bool isValid);
    void uuidChanged(QString uuid);
    void nameChanged(QString name);
    void picPathChanged(QString path);
    void ratingChanged(double rating);
    void personCountChanged(int count);
    void picRotationChanged(int rotation);

private:
    bool mValid = false;
    int mPersonCount = 1;
    MealItem *mItem = nullptr;

};

#endif // WEEKVIEWITEM_H
