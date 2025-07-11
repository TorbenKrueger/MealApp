#ifndef MEALLISTMODEL_H
#define MEALLISTMODEL_H

#include <QAbstractListModel>

class MealList;
class MealItem;
class MealListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MealList *list READ list WRITE setList)

public:
    explicit MealListModel(QObject *parent = nullptr);

    enum {
        uuidRole,
        nameRole,
        ratingRole,
        picRole,
        picRotaionRole,
        objectRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    MealList *list() const;
    void setList(MealList *list);

private:
    MealList *mList;
};

#endif // MEALLISTMODEL_H
