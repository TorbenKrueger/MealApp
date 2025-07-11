#ifndef MEALLIST_H
#define MEALLIST_H

#include <QObject>
#include <QVector>

#include "mealitem.h"
#include "meallistmodel.h"

#include <QJsonObject>
#include <QJsonArray>

class MealItem;
class ServerConnection;
class MealList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

    Q_PROPERTY(QStringList backups READ backups WRITE setBackups NOTIFY backupsChanged)

    Q_PROPERTY(bool orderAsc READ orderAsc WRITE setOrderAsc NOTIFY orderAscChanged)
    Q_PROPERTY(QString orderType READ orderType WRITE setOrderType NOTIFY orderTypeChanged)

    Q_PROPERTY(int styleMode READ styleMode WRITE setStyleMode NOTIFY styleChanged)
    Q_PROPERTY(QString primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY styleChanged)
    Q_PROPERTY(QString secondaryColor READ secondaryColor WRITE setSecondaryColor NOTIFY styleChanged)
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY styleChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor NOTIFY styleChanged)

    Q_PROPERTY(bool lunch READ lunch WRITE setLunch NOTIFY lunchChanged)
    Q_PROPERTY(bool dessert READ dessert WRITE setDessert NOTIFY dessertChanged)
    Q_PROPERTY(bool vegetarian READ vegetarian WRITE setVegetarian NOTIFY vegetarianChanged)
    Q_PROPERTY(bool canVegetarian READ canVegetarian WRITE setCanVegetarian NOTIFY canVegetarianChanged)

public:
    explicit MealList(QObject *parent = nullptr);

    QVector<MealItem*> items() const;

    bool setItemAt(int index, MealItem *item);

    MealItem* getItemByUuid(const QString &uuid);

signals:
    void styleChanged(int styleMode);

    void propertyChanged(const QString &uuid);

    void dataChanged(int index);

    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void removedItem(const QString &uuid);

    void error(QString text);

    void countChanged(int count);
    void backupsChanged(QStringList list);

    void orderAscChanged();
    void orderTypeChanged();

    void lunchChanged(bool b);
    void dessertChanged(bool b);
    void vegetarianChanged(bool b);
    void canVegetarianChanged(bool b);

    void close();
    void newMealFromServer(QString uuid, QString name, QString date);

    void loadedBackup(QJsonArray ary);

public slots:
    bool appendItem(MealItem *item);
    void removeAllItems();
    void removeItem(int index);
    void removeItem(MealItem *item);

    Q_INVOKABLE MealItem *createAndAppend(const QString name = QString());
    Q_INVOKABLE void order(const QString &type, bool asc = true);
    Q_INVOKABLE void createBackup(const QString &name);
    Q_INVOKABLE void loadBackup(const QString &name);
    Q_INVOKABLE int count();
    Q_INVOKABLE MealItem* item(int index);
    Q_INVOKABLE QString formatFile(const QString &file);
    Q_INVOKABLE void resetAll();

    Q_INVOKABLE void sendAllToServer(const QString &ip = QString(), int port = -1);
    Q_INVOKABLE void getAllFromServer(const QString &ip = QString(), int port = -1);
    Q_INVOKABLE void loadBackupFromServer(const QString &backup);
    Q_INVOKABLE void acceptedServerMeal(const QString &uuid);
    Q_INVOKABLE void nextServerMeal(const QString &uuid);

    void saveStyle(int style = 0);

    int styleMode() const;
    void setStyleMode(int mode);

    QString primaryColor() const;
    void setPrimaryColor(const QString &color);

    QString secondaryColor() const;
    void setSecondaryColor(const QString &color);

    QString textColor() const;
    void setTextColor(const QString &color);

    QString highlightColor() const;
    void setHighlightColor(const QString &color);

    QString orderType() const;
    void setOrderType(const QString &type);

    bool orderAsc() const;
    void setOrderAsc(bool asc);

    QStringList backups();
    void setBackups(const QStringList &list);

    bool lunch() const;
    void setLunch(bool b);

    bool dessert() const;
    void setDessert(bool b);

    bool vegetarian() const;
    void setVegetarian(bool b);

    bool canVegetarian() const;
    void setCanVegetarian(bool b);

private:
    enum class OrderTypes
    {
        Name = 1,
        CreationDate = 2,
        LastCooked = 3,
        Rating = 4
    };

    //    struct QPairFirstComparer
    //    {
    //        template<typename T1, typename T2>
    //        bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    //        {
    //            return a.first < b.first;
    //        }
    //    };

    //    struct QPairSecondComparer
    //    {
    //        template<typename T1, typename T2>
    //        bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    //        {
    //            return a.second < b.second;
    //        }
    //    };

    ServerConnection *mServerConnection = nullptr;
    QString mIp = "192.168.0.95";
    int mPort = 1337;

    QTimer *mSaveTimer;
    QVector<MealItem*> mItems;
    void loadMeals(const QString &location = QString(), const QString &doc = "");
    int indexOfItem(MealItem *item);

    int mStyleMode = 0;
    QString mPriColor;
    QString mSecColor;
    QString mTextColor;
    QString mHighColor;

    QString mOrderType = "name";
    bool mOrderAsc = true;

    bool mLunch = true;
    bool mDessert = true;
    bool mVegetarian = true;
    bool mCanVegetarian = true;

    QStringList mBackups;

    void sortMeals(OrderTypes type, bool asc);
    void saveOrderTypes();
    void saveCategories();
    void loadCategories();
private slots:
    void showServerMeal(const QJsonObject &obj);
    void saveMeals(const QString &location = QString());

    void saveAfterTimeout(const QString &location = QString());
    void loadServerBackup(QJsonArray ary);
};

#endif // MEALLIST_H
