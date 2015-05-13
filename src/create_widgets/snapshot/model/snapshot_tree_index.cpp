#include "snapshot_tree_index.h"


TreeItem::TreeItem(const QString &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    itemDate = QString("Date");
    state = false;
}

void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

void TreeItem::removeChild(TreeItem *item)
{
    childItems.removeOne(item);
}

TreeItem *TreeItem::child(int row)
{
    return childItems.value(row, NULL);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
    return 1;
}

QVariant TreeItem::data(int column) const
{
    if ( column ) return itemDate;
    else return itemData;
}

void TreeItem::setData(QString &data)
{
    itemData = data;
}

void TreeItem::setDate(QString &date)
{
    itemDate = date;
}

bool TreeItem::getState() const
{
    return state;
}

void TreeItem::setState(bool _state)
{
    state = _state;
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
