#ifndef DISK_SNAPSHOT_H
#define DISK_SNAPSHOT_H

#include "create_widgets/snapshot/_snapshot_stuff.h"

class DiskSnapshot : public _SnapshotStuff
{
    Q_OBJECT
public:
    explicit DiskSnapshot(
            QWidget *parent = Q_NULLPTR,
            bool     _state = false,
            bool     _external = false);

public slots:
    QDomDocument     getElements() const;
};

#endif // DISK_SNAPSHOT_H
