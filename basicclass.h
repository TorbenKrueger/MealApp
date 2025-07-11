#ifndef BASICCLASS_H
#define BASICCLASS_H

#include <QObject>

class BasicClass : public QObject
{
    Q_OBJECT
public:
    explicit BasicClass(QObject *parent = nullptr);
    ~BasicClass();

    void simulateTouch();

    static QString createUUID(const QString &name);
    static QString dateFormat();
    static QString umlauts(QString text, bool aeToA = true);

public slots:
    bool eventFilter(QObject *obj, QEvent *evt);
};

#endif // BASICCLASS_H
