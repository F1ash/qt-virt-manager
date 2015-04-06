#ifndef REVERT_SNAPSHOT_FLAGS_H
#define REVERT_SNAPSHOT_FLAGS_H

#include "_snapshot_flags.h"

class RevertSnapshotFlags : public _SnapshotFlags
{
    Q_OBJECT
public:
    RevertSnapshotFlags(
            QWidget     *parent = NULL);

public slots:
    void             changeAvailableFlags(int);

private slots:
    void             changeFlagCompatibility();
};

#endif // REVERT_SNAPSHOT_FLAGS_H
