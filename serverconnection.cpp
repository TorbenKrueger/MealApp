#include "serverconnection.h"

#include <QDebug>
#include <QSettings>

ServerConnection::ServerConnection(TcpSocket *socket)
{
    mSocket = socket;

    connect(socket, &TcpSocket::newTcpPackage, this, [=](QByteArray data)
    {
        qDebug() << "Daten erhalten:" << TcpSocket::getText(data);

        if (TcpSocket::getType(data) == "avbservermeals") {
            QJsonDocument doc = QJsonDocument::fromJson(TcpSocket::getText(data).toLatin1());
            if (doc.isObject()) {
                mealFromServerReceived(doc.object());
            }
            else if (doc.isArray()) {
                const auto array = doc.array();
                for (int i = 0; i < array.count(); i++)
                    mealFromServerReceived(array.at(i).toObject());
            }
            else {
                qDebug() << doc.isNull() << doc;
            }
        }
        else if (TcpSocket::getType(data) == "servermeal") {
            QJsonDocument doc = QJsonDocument::fromJson(TcpSocket::getText(data).toLatin1());
            if (doc.isObject()) {
                emit addNewMeal(doc.object());
            }
            else {
                qDebug() << "Fehler";
            }
        }
        else if (TcpSocket::getType(data) == "loadbackup") {
            QJsonDocument doc = QJsonDocument::fromJson(TcpSocket::getText(data).toLatin1());
            if (doc.isArray()) {
                emit loadedBackup(doc.array());
            }
            else {
                qDebug() << "Fehler";
            }
        }
    });
    connect(socket, &TcpSocket::connectionLost, this, [=]()
    {
        qDebug()<< "Verbindung verloren";
    });

    mSocket->sendData("client", 1, enumToString(CommunicationTypes::AvbServerMeals), "");
}

void ServerConnection::mealFromServerReceived(const QJsonObject &obj)
{
    qDebug() << "Vom Server erhalten:" << obj.value("mealName");

    if (mServerMealStack.isEmpty())
        showServerMeal(obj);

    mServerMealStack.append(obj);
}

void ServerConnection::removeFromStack(const QString &uuid)
{
    for (int i = 0; i < mServerMealStack.length(); i++) {
        if (mServerMealStack.at(i).value("uuid") == uuid) {
            mServerMealStack.removeAt(i);
            break;
        }
    }
}

void ServerConnection::next(const QString &prevUuid, bool accepted)
{
    removeFromStack(prevUuid);
    if (!mServerMealStack.isEmpty())
        emit showServerMeal(mServerMealStack.first());

    if (accepted) {
        // Ganzes Essen vom Server holen
        mSocket->sendData("Client", 0, enumToString(CommunicationTypes::ServerMeal), prevUuid);
    }
}

QString ServerConnection::enumToString(CommunicationTypes type) const
{
    switch (type) {
    case CommunicationTypes::Init:
        return "init";
        break;
    case CommunicationTypes::AvbServerMeals:
        return "avbservermeals";
        break;
    case CommunicationTypes::ServerMeal:
        return "servermeal";
        break;
    case CommunicationTypes::Backup:
        return "backup";
        break;
    case CommunicationTypes::LoadBackup:
        return "loadbackup";
        break;
    case CommunicationTypes::Text:
        return "text";
        break;
    }
}

void ServerConnection::backup() const
{
    const QString type = enumToString(CommunicationTypes::Backup);

    QString location = "meals";
    QSettings settings("krueger", "mealApp");
    const auto data = settings.value(location).toByteArray();

    mSocket->sendData("Client", 1, type, data);
}

void ServerConnection::loadBackup() const
{
    const QString type = enumToString(CommunicationTypes::LoadBackup);
    mSocket->sendData("Client", 1, type, "");
}
