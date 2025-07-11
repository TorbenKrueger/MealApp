#ifndef MEALITEM_H
#define MEALITEM_H

#include <QObject>
#include <QWidget>
#include <QDate>

#include "mealproperty.h"

class MealItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString picturePath READ picturePath WRITE setPicturePath NOTIFY picturePathChanged)
    Q_PROPERTY(QString lastCooked READ lastCooked WRITE setLastCooked NOTIFY lastCookedChanged)
    Q_PROPERTY(double rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int personCount READ persCount WRITE setPersCount NOTIFY personCountChanged)
    Q_PROPERTY(QString creationDate READ creationDate)
    Q_PROPERTY(int pictureRotation READ rotation WRITE setRotation NOTIFY pictureRotationChanged)
    // 0: "Mittagessen", 1: "Nachtisch", 2: "Vegetarisch", 3: "Geht auch vegetarisch"]
    Q_PROPERTY(int categorie READ categorie WRITE setCategorie NOTIFY categorieChanged)

    Q_PROPERTY(MealProperties* properties READ properties NOTIFY propertiesChanged)

public:
    MealItem(QObject *parent = nullptr);
    MealItem(const QJsonObject &jsonObj, QObject *parent = nullptr);
    MealItem(const QString &uuid, const QString &name, const QString &picturePath, const QString &description, QObject *parent = nullptr);

    QString uuid() const;
    void setUuid(const QString &uuid);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    QString picturePath() const;
    void setPicturePath(const QString &path);

    QString lastCooked();
    void setLastCooked(const QString &date = QString());
    void setLastCooked(const QDate &date = QDate::currentDate());

    double rating() const;
    void setRating(double rating);

    int persCount();
    void setPersCount(int cnt);

    QString creationDate() const;

    int rotation() const;
    void setRotation(int rotation);

    int categorie() const;
    void setCategorie(int cat);

    Q_INVOKABLE bool addProperty(const QString &text, int persCount = 1);
    Q_INVOKABLE bool addProperty(double quantity, const QString &unit, const QString &name);
    Q_INVOKABLE void removeProperty(const QString &name);
    Q_INVOKABLE void removeAllProperties();

    Q_INVOKABLE QString categorieText();

    Q_INVOKABLE QString getAllProperties();

    QList<QStringList> getProperties();
    void remveAllProperties();

    Q_INVOKABLE void remove(); // remove itself
    Q_INVOKABLE bool filter(const QString &filter);
    Q_INVOKABLE bool categorieType(const QList<int> &types);

    Q_INVOKABLE void save();

    Q_INVOKABLE void savePicture(const QString &path);

    QJsonObject getJsonObject();

    MealProperties* properties();

    Q_INVOKABLE void update();
signals:
    void toRemove(MealItem *item);

    void dataChanged();
    void nameChanged(QString name);
    void descriptionChanged(QString description);
    void picturePathChanged(QString path);
    void lastCookedChanged(QString date);
    void ratingChanged(double rating);
    void personCountChanged(int count);
    void uuidChanged(QString uuid);
    void pictureRotationChanged(int rotation);
    void categorieChanged(int cat);

    void propertiesChanged();

    void saveAll();

private:
    MealProperties *mProperties = nullptr;

    double mRating = 0;
    QString mUuid;
    QString mName;
    QString mPicturePath = QString();
    QString mDescription;
    int mPersCount = 2;
    int mRotation = 90;
    int mCategorie = 0;

    QString mTmpPicturePath = QString();

    QDate mCreationDate = QDate::currentDate();
    QDate mLastCookedDate;

    QByteArray getPictureData(const QString &path);
};

#endif // MEALITEM_H
