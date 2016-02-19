#ifndef CREATE_SNAPSHOT_FLAGS_H
#define CREATE_SNAPSHOT_FLAGS_H

#include "_snapshot_flags.h"

class CreateSnapshotFlags : public _SnapshotFlags
{
    Q_OBJECT
public:
    explicit CreateSnapshotFlags(
            QWidget     *parent = nullptr);

public slots:
    void             changeAvailableFlags(int);

private slots:
    void             changeFlagCompatibility();
};

#endif // CREATE_SNAPSHOT_FLAGS_H
