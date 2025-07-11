#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>

class QTcpSocket;
class QTextStream;
class TcpSocket : public QObject
{
	Q_OBJECT
public:
    explicit TcpSocket(QTcpSocket *socket = nullptr, QObject *parent = nullptr);

    void setName(const QString &name);

	bool connectToHost(QString ip, int port);
	bool disconnectFromHost();
	bool sendData(QString source, int id, QString typ, QString data);
	bool sendData(QString source, int id, QString typ, QStringList data);

    void setSocket(QTcpSocket *pendingSocket);
    QString name() const;
    bool write(const QString &msg);

    static QString getText(const QByteArray &rawdata);
    static QString getSource(const QByteArray &rawdata);
    static QString getType(const QByteArray &rawdata);
    static int getId(const QByteArray &rawdata);
    static QJsonObject getObject(const QByteArray &rawdata);

signals:
	void newTcpPackage(QByteArray data);
	void connectionLost();

private slots:
	void getData();
	void socketDisCon(); // Verbindung verloren

private:
    int id = 0;
    int m_packageSize = -1;
	bool m_packageSizeReceive = false;
	bool m_packageComplete = false;
	QTcpSocket *m_socket = nullptr;
	QByteArray m_package;

    QString m_name;

	int getPos(QByteArray data);
	bool newPackage(int pos);
	void splitPackage(QByteArray data);
	void newData();
	void packageSize();
	QByteArray size(QByteArray data);
	QByteArray convert(QJsonObject obj);
};

#endif // TCPSOCKET_H
