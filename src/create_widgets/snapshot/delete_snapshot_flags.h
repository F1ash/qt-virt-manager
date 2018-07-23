#ifndef DELETE_SNAPSHOT_FLAGS_H
#define DELETE_SNAPSHOT_FLAGS_H

#include "_snapshot_flags.h"

class DeleteSnapshotFlags : public _SnapshotFlags
{
    Q_OBJECT
public:
    DeleteSnapshotFlags(
            QWidget     *parent = Q_NULLPTR);

public slots:
    void             changeAvailableFlags(int);

private slots:
    void             changeFlagCompatibility();
};

#endif // DELETE_SNAPSHOT_FLAGS_H
