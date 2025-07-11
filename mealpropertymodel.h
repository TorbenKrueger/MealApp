#ifndef MEALPROPERTYMODEL_H
#define MEALPROPERTYMODEL_H

#include <QAbstractListModel>

class MealProperties;

class MealPropertyModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MealProperties *list READ list WRITE setList)

public:
    explicit MealPropertyModel(QObject *parent = nullptr);

    enum {
        uuidRole,
        quantityRole,
        unitRole,
        nameRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    MealProperties *list() const;
    void setList(MealProperties *list);

    Q_INVOKABLE QString getAllItems();
private:
    MealProperties *mList;
};

#endif // MEALPROPERTYMODEL_H
