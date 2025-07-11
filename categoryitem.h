#ifndef CATEGORYITEM_H
#define CATEGORYITEM_H

#include <QObject>
#include <QJsonObject>

class CategoryItem : public QObject
{
    Q_OBJECT
public:
    explicit CategoryItem(const QString &name, QObject *parent = nullptr);

    void setIsBeef(bool isBeef = true);
    void setIsVegan(bool isVegan = true);

    bool isBeef() const;
    bool isVegetable() const;
    bool isVegan() const;

    QString name();
    void setJson(const QJsonObject &json);
    QJsonObject getJson();
signals:

private:
    QString mName;
    bool mIsBeef = true;
    bool mIsVegan = false;

};

#endif // CATEGORYITEM_H
