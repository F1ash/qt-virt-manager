#ifndef _SNAPSHOT_FLAGS_H
#define _SNAPSHOT_FLAGS_H

#include <QMenuBar>
#include <QActionGroup>
#include <QDebug>
#include <math.h>

class _SnapshotFlags : public QMenu
{
    Q_OBJECT
public:
    explicit _SnapshotFlags(
            QWidget     *parent = nullptr);
    QActionGroup    *actGroup;
    int              getCompositeFlag() const;
    virtual void     changeAvailableFlags(int);
    virtual void     changeFlagCompatibility();
};

#endif // _SNAPSHOT_FLAGS_H
