#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QIcon>

enum TreeItemType { ROOT, FOLDER, VIDEO, IMAGE, OTHER, NONE };

class TreeItem {
public:
    TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr, int size = 0, QString path = QString(""), QString hash = QString(), TreeItemType type = ROOT);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant dataValue(int column) const;
    int row() const;
    TreeItem *parentItem();
    TreeItemType getType();
    QString getPath();
    QString getHash();
    int getSize();

private:
    QVector<TreeItem*> children;
    QVector<QVariant> data;
    TreeItem *parent;

    QString path;
    QString hash;
    TreeItemType type;
    int size;
};

class ItemModel : public QAbstractItemModel
{
public:
    ItemModel(TreeItem *root);
    ~ItemModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    TreeItem *rootItem;
};

#endif // ITEMMODEL_H
