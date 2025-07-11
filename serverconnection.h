#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>
#include "tcpsocket.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class ServerConnection : public QObject
{
    Q_OBJECT

public:
    ServerConnection(TcpSocket *socket);

    enum class CommunicationTypes
    {
        Init = 1,
        AvbServerMeals = 2,
        ServerMeal = 3,
        Backup = 4,
        LoadBackup = 5,
        Text = 6
    };

public:
    void next(const QString &prevUuid, bool accepted = false);
    void backup() const;

    void loadBackup() const;
signals:
    void addNewMeal(QJsonObject obj);
    void showServerMeal(QJsonObject obj);
    void loadedBackup(QJsonArray ary);

private:
    QString enumToString(CommunicationTypes type) const;

    TcpSocket *mSocket = nullptr;
    QList<QJsonObject> mServerMealStack;

    void mealFromServerReceived(const QJsonObject &obj);
    void removeFromStack(const QString &uuid);
};

#endif // SERVERCONNECTION_H
