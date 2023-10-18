#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractItemModel>

class ItemModel : public QAbstractItemModel
{
public:
    ItemModel();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // ITEMMODEL_H
