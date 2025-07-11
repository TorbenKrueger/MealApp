#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QObject>
#include <QVector>

struct Item
{
    bool checked = false;
    double quantity = 0.0;
    QString unit;
    QString name;
};

class ItemList : public QObject
{
    Q_OBJECT
public:
    explicit ItemList(QObject *parent = nullptr);

    QVector<Item> items() const;

    bool setItemAt(int index, const Item &item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    bool appendItem(bool checked, double quantity, const QString &unit, const QString &name);
    void removeAllItems();
    void removeItem(int index);

private:
    QVector<Item> mItems;
};

#endif // ITEMLIST_H
