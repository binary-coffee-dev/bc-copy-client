#include "itemmodel.h"

/*
 * TREE ITEM
*/

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent, int size, QString path, QString hash, TreeItemType type)
    : data(data), parent(parent), size(size), path(path), hash(hash), type(type)
{}

TreeItem::~TreeItem()
{
    qDeleteAll(children);
}

void TreeItem::appendChild(TreeItem *child)
{
    this->children.append(child);
}

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= children.size()) {
        return nullptr;
    }
    return children.at(row);
}

int TreeItem::childCount() const
{
    return children.size();
}

int TreeItem::columnCount() const
{
    return data.size();
}

QVariant TreeItem::dataValue(int column) const
{
    return data.at(column);
}

int TreeItem::row() const
{
    if (parent) {
        parent->children.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}

TreeItem *TreeItem::parentItem()
{
    return parent;
}

TreeItemType TreeItem::getType()
{
    return type;
}

QString TreeItem::getPath()
{
    return path;
}

QString TreeItem::getHash()
{
    return hash;
}

int TreeItem::getSize()
{
    return size;
}


/*
 * ITEM MODEL
 */

ItemModel::ItemModel(TreeItem *root)
{
    rootItem = root;
}

ItemModel::~ItemModel()
{
    delete rootItem;
}

QModelIndex ItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) ) {
        return QModelIndex();
    }

    TreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex ItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int ItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }
    return rootItem->columnCount();
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if( role == Qt::DisplayRole) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        return item->dataValue(index.column());
    }

    if ( role == Qt::DecorationRole && index.column() == 0) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        switch (item->getType()) {
        case FOLDER:
            return QIcon(":/tree/icons/folder.png");
        case VIDEO:
            return QIcon(":/tree/icons/clapperboard-play.png");
        case IMAGE:
            return QIcon(":/tree/icons/picture.png");
        case OTHER:
            return QIcon(":/tree/icons/document.png");
        default:
            break;
        }

    }

    return QVariant();
}

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index);
}

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->dataValue(section);
    }
    return QVariant();
}
