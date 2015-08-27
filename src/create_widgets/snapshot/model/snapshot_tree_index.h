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
    QString              getDesc() const;
    void                 setData(QString&);
    void                 setDate(QString&);
    bool                 getState() const;
    void                 setState(bool);
    void                 setDesc(QString&);
    int                  row() const;
    TreeItem            *parent();

private:
    QList<TreeItem*>     childItems;
    QString              itemData, itemDate, itemDesc;
    bool                 state;
    TreeItem            *parentItem;
};

#endif // SNAPSHOT_TREE_INDEX_H
