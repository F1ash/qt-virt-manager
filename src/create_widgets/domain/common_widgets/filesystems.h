#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H

#include <QStackedWidget>
#include "fs_type_widgets/mount_fstype.h"
#include "fs_type_widgets/file_fstype.h"
#include "fs_type_widgets/block_fstype.h"
#include "fs_type_widgets/ram_fstype.h"
#include "fs_type_widgets/bind_fstype.h"

class FileSystems : public _QWidget
{
    Q_OBJECT
public:
    explicit FileSystems(
            QWidget        *parent      = NULL,
            virConnectPtr*  connPtrPtr  = NULL);
    QComboBox       *type;
    QStackedWidget  *info;

private:
    QLabel          *typeLabel;
    QHBoxLayout     *typeLayout;
    QWidget         *typeWdg;
    QVBoxLayout     *commonLayout;

    QString          connType;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             setWidgets(QString);
};

#endif // FILESYSTEMS_H
