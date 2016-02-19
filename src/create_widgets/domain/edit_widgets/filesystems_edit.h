#ifndef FILESYSTEMS_EDIT_H
#define FILESYSTEMS_EDIT_H

#include "create_widgets/domain/common_widgets/filesystems.h"

class FileSystems_Edit : public FileSystems
{
    Q_OBJECT
public:
    explicit FileSystems_Edit(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);

signals:

public slots:
    void         setDataDescription(QString&);
};

#endif // FILESYSTEMS_EDIT_H
