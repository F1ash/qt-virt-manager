#ifndef MEM_STATE_SNAPSHOT_H
#define MEM_STATE_SNAPSHOT_H

#include "create_widgets/snapshot/_snapshot_stuff.h"

class MemStateSnapshot : public _SnapshotStuff
{
    Q_OBJECT
public:
    explicit MemStateSnapshot(
            QWidget *parent = NULL,
            bool     _state = false);

private:
    QLabel  *info;

public slots:
};

#endif // MEM_STATE_SNAPSHOT_H
