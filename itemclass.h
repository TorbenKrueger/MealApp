#ifndef ITEMCLASS_H
#define ITEMCLASS_H

#include <QObject>
#include<QVector>

struct Item {
    bool isChecked;
    int quantity;
    QString unit;
    QString name;
};


class ItemClass : public QObject
{
    Q_OBJECT

public:
    explicit ItemClass(QObject *parent = nullptr);

    QVector<Item> items() const;
    bool setItemAt(int index, const Item &item);


signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void removeItem(int index);

private:
    QVector<Item> mItems;
};

#endif // ITEMCLASS_H
