#ifndef MEALPROPERTY_H
#define MEALPROPERTY_H

#include <QObject>
#include <QMap>

class MealProperty;

class MealProperties : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QList<MealProperty*> properties READ property)
//    Q_PROPERTY(QVector<MealProperty*> list READ list)

public:
    explicit MealProperties(QObject *parent = nullptr);

    QList<MealProperty*> property() const;
    QList<MealProperty*> items() const;

    QVector<MealProperty*> list() const;

//    void setProperty(const QString &uuid, double count, const QString &unit, const QString &name);
//    void setProperty(const QString &uuid, MealProperty *property);

    void removeProperty(const QString &uuid);

    void removeAll();
    bool addProperty(const QString &uuid, double count = 0, const QString &unit = QString(), const QString &name = QString());

signals:
    void dataChanged(int index);

    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

private:
    QList<MealProperty *> mProperties;
    int indexOfItem(MealProperty *item);
    MealProperty *uuidToItem(const QString &uuid);
};

class MealProperty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(double count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit MealProperty(QObject *parent, const QString &uuid);

    QString uuid() const;
    void setUuid(const QString &uuid);

    double count() const;
    void setCount(double count);

    QString unit() const;
    void setUnit(const QString &unit);

    QString name() const;
    void setName(const QString &name);

signals:
    void dataChanged();

    void uuidChanged(QString uuid);
    void countChanged(int count);
    void unitChanged(QString unit);
    void nameChanged(QString name);

private:
    QString mUuid = QString();
    double mCount = 0;
    QString mUnit = QString();
    QString mName = QString();
};

#endif // MEALPROPERTY_H
