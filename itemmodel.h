#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractListModel>

class ItemList;

class ItemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ItemList *list READ list WRITE setList)

public:
    explicit ItemModel(QObject *parent = nullptr);

    enum {
        checkedRole,
        quantityRole,
        unitRole,
        nameRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    ItemList *list() const;
    void setList(ItemList *list);

    Q_INVOKABLE QString getAllItems();
private:
    ItemList *mList;
};

#endif // ITEMMODEL_H
