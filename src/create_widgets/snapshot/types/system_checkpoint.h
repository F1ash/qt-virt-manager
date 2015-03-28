#ifndef SYSTEM_CHECKPOINT_H
#define SYSTEM_CHECKPOINT_H

#include "create_widgets/snapshot/_snapshot_stuff.h"

class SystemCheckpoint : public _SnapshotStuff
{
    Q_OBJECT
public:
    SystemCheckpoint(
            QWidget *parent = NULL,
            bool     _state = false,
            bool     _external = false);

private:
    const bool       external;

public slots:
    QDomDocument     getElements() const;
};

#endif // SYSTEM_CHECKPOINT_H
