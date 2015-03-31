#ifndef SNAPSHOT_TREE_INDEX_H
#define SNAPSHOT_TREE_INDEX_H

#include <QObject>

class SnapshotTreeIndex : public QObject
{
    Q_OBJECT
public:
    explicit SnapshotTreeIndex(QObject *parent = 0);

signals:

private:
    QString Name;

public slots:
    void    setName(const QString&);
    QString getName() const;
};

#endif // SNAPSHOT_TREE_INDEX_H
