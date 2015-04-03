#ifndef SNAPSHOT_TREE_INDEX_H
#define SNAPSHOT_TREE_INDEX_H

#include <QString>
#include <QVariant>

class TreeItem
{
public:
    TreeItem(const QString &data, TreeItem *parent = NULL);

    void                 appendChild(TreeItem *child);
    void                 removeChild(TreeItem *child);

    TreeItem            *child(int row);
    int                  childCount() const;
    int                  columnCount() const;
    QVariant             data(int column) const;
    void                 setData(QString&);
    bool                 getState() const;
    void                 setState(bool);
    int                  row() const;
    TreeItem            *parent();

private:
    QList<TreeItem*>     childItems;
    QString              itemData;
    bool                 state;
    TreeItem            *parentItem;
};

#endif // SNAPSHOT_TREE_INDEX_H
