#ifndef SHOPPINGLISTMODEL_H
#define SHOPPINGLISTMODEL_H

#include <QAbstractListModel>

class ShoppingList;

class ShoppingListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ShoppingList *list READ list WRITE setList)

public:
    explicit ShoppingListModel(QObject *parent = nullptr);

    enum {
        buyedRole,
        quantityRole,
        unitRole,
        nameRole,
        addedAtRole,
        buyedAtRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    ShoppingList *list() const;
    void setList(ShoppingList *list);

private:
    ShoppingList *mList;

};

#endif // SHOPPINGLISTMODEL_H
