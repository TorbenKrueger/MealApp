#include "tcpsocket.h"

#include <QTcpSocket>
#include <QDebug>
#include <QTextStream>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QDateTime>
#include <QTimer>

#include <QFile>

TcpSocket::TcpSocket(QTcpSocket *socket, QObject *parent) : QObject(parent)
{
    if (socket)
        setSocket(socket);
    else
        m_socket = new QTcpSocket(this);
}

void TcpSocket::setName(const QString &name)
{
    m_name = name;
}

QString TcpSocket::name() const
{
    return m_name;
}

void TcpSocket::setSocket(QTcpSocket *pendingSocket)
{
    m_socket = pendingSocket;
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(getData()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(socketDisCon()));
}

bool TcpSocket::connectToHost(QString ip, int port)
{
    bool ok = false;
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        disconnectFromHost();

    m_socket->connectToHost(ip, port);
    ok = m_socket->waitForConnected();

    if (ok && m_socket->isOpen()) {
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(getData()));
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(socketDisCon()));
        write("connected at: " + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss:zzz"));
    }
    else {
        qDebug() << m_socket->errorString();
        ok = false;
    }
    return ok;
}
bool TcpSocket::disconnectFromHost()
{
    bool ok = false;
    if (m_socket) {
        m_socket->waitForBytesWritten();
        QTimer::singleShot(250, this, [=](){m_socket->disconnectFromHost();});
        ok = true;
    }
    return ok;
}

bool TcpSocket::write(const QString &msg)
{
    return sendData("client", id++, "text", msg);
}

bool TcpSocket::sendData(QString source, int id, QString typ, QStringList data)
{
    bool ok = false;
    if (m_socket && m_socket->isValid() && m_socket->state() == QAbstractSocket::ConnectedState) {

        QJsonObject obj;
        obj["source"] = source;
        obj["id"] = id;
        obj["type"] = typ;

        QJsonArray ary;
        ary = QJsonArray::fromStringList(data);
        obj["data"] = ary;

        m_socket->write(size(convert(obj)));
        m_socket->write(convert(obj));
        ok = m_socket->waitForBytesWritten();
    }
    else {
        ok = false;
    }
    return ok;
}
bool TcpSocket::sendData(QString source, int id, QString typ, QString data)
{
    bool ok = false;
    if (m_socket && m_socket->isValid() && m_socket->state() == QAbstractSocket::ConnectedState) {

        QJsonObject obj;
        obj["source"] = source;
        obj["id"] = id;
        obj["type"] = typ;
        obj["data"] = data;

        m_socket->write(size(convert(obj)));
        m_socket->write(convert(obj));
        ok = m_socket->waitForBytesWritten();
    }
    else {
        ok = false;
    }
    return ok;
}
QByteArray TcpSocket::size(QByteArray data)
{
    int l = data.length();
    QByteArray ba(QString::number(l).toLatin1());
    ba += ":";
    return ba;
}

QByteArray TcpSocket::convert(QJsonObject obj)
{
    QJsonDocument doc;
    doc.setObject(obj);

    return doc.toJson();
}

void TcpSocket::packageSize()
{
    if (m_package.length() == m_packageSize) {
        // Paket war in einem Paket
        m_packageComplete = true;
    }
    else if (m_package.length() > m_packageSize) {
        // Paket besteht aus zwei oder mehr Paketen
        splitPackage(m_package);
    }
    else {
        // Paket unvollständing -> warte auf weitere TCP-Pakete
    }
}

bool TcpSocket::newPackage(int pos)
{
    bool ok = false;
    if (pos == -1) {
        m_packageComplete = false;
        m_packageSizeReceive = false;
        m_packageSize = -1;
        return ok;
    }
    else {
        QByteArray copy = m_package;
        m_packageSize = copy.remove(pos, copy.length()).toInt(&ok);

        qDebug() << "TcpSocket" << "PackageSize" << m_packageSize;

        if (ok) {
            m_package = m_package.remove(0, pos + 1);// +1 für den Doppelpunkt
            m_packageSizeReceive = true;
            qDebug() << "TcpSocket" << "PACKJED received" << m_package;
        }
        else {
            m_packageComplete = false;
            m_packageSizeReceive = false;
            m_packageSize = -1;
            m_package.clear();
        }
    }
    return ok;
}

void TcpSocket::newData()
{
    if (!m_packageSizeReceive) {
        if (m_package.contains(":")) {
            int pos = getPos(m_package);

            qDebug() << "TcpSocket" << "Final pos" << pos;

            if (newPackage(pos)) {
                // alles gut
                packageSize();
            }
            else {
                //Fehler
                return;
            }
        }
        else {
            qDebug() << "TcpSocket" << "Kein DoppelPunkt";
        }
    }
    else {
        packageSize();
    }
}

void TcpSocket::getData()
{
    if (m_socket) {
        QByteArray data = m_socket->readAll();

        qDebug() << "TcpSocket" << data;

        m_package.append(data);
        qDebug() << "TcpSocket" << m_package;
        newData();

        if (m_packageComplete) {
            //Paket vollständig -> variable zurücksetzen
            id = getId(m_package) + 1;
            emit newTcpPackage(m_package);

            qDebug() << "TcpSocket" << "paket complett1";

            QFile file("D:\\torbe\\Documents\\tmpDebug.txt");
            file.open(QFile::ReadWrite | QFile::Text);

            file.write(m_package);
            file.close();

            m_packageComplete = false;
            m_packageSizeReceive = false;
            m_packageSize = -1;
            m_package.clear();
        }
    }
}

void TcpSocket::splitPackage(QByteArray data)
{
    QList <QByteArray> packages;
    while (!data.isEmpty() && m_packageSize <= data.length()) {
        QByteArray copy = data;
        packages << copy.remove(m_packageSize, copy.length());
        data = data.remove(0, m_packageSize);
        int pos = getPos(data);
        if (pos == -1) {
            m_packageComplete = false;
            m_packageSizeReceive = false;
            m_packageSize = -1;
            break;
        }
        else {
            bool ok;
            QByteArray copy = data;
            m_packageSize = copy.remove(pos, copy.length()).toInt(&ok);
            if (!ok)break;
            data.remove(0, pos + 1);
        }
    }

    m_package = data;
    for (int i = 0; i < packages.length(); i++) {
        id = getId(packages[i]) + 1;
        qDebug() << "TcpSocket" << "paket complett2";
        emit newTcpPackage(packages[i]);
    }
}

int TcpSocket::getPos(QByteArray data)
{
    int pos = -1;

    for (int i = 0; i < data.length(); i++) {
        if (data.at(i) == ':') {
            pos = i;
            break;
            qDebug() << "TcpSocket" << data.at(i);
        }
    }
    bool ok;
    data.remove(pos, data.length()).toInt(&ok);
    if (!ok)
        pos = -1; //error
    return pos;
}


void TcpSocket::socketDisCon()
{
    emit connectionLost();
}

QString TcpSocket::getText(const QByteArray &rawdata)
{
    return getObject(rawdata).value("data").toString();
}

QString TcpSocket::getSource(const QByteArray &rawdata)
{
    return getObject(rawdata).value("source").toString();
}

QString TcpSocket::getType(const QByteArray &rawdata)
{
    return getObject(rawdata).value("type").toString();
}

int TcpSocket::getId(const QByteArray &rawdata)
{
    return getObject(rawdata).value("id").toInt();
}

QJsonObject TcpSocket::getObject(const QByteArray &rawdata)
{
    QJsonDocument doc = QJsonDocument::fromJson(rawdata);
    QJsonObject obj = doc.object();

    return obj;
}
