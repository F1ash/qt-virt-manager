#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H

#include <QStackedWidget>
#include "fs_type_widgets/mount_fstype.h"
#include "fs_type_widgets/file_fstype.h"
#include "fs_type_widgets/block_fstype.h"
#include "fs_type_widgets/ram_fstype.h"
#include "fs_type_widgets/bind_fstype.h"
#include "create_widgets/common/_qwidget_threaded.h"

class FileSystems : public _QWidget_Threaded
{
    Q_OBJECT
public:
    explicit FileSystems(
            QWidget        *parent      = Q_NULLPTR,
            virConnectPtr*  connPtrPtr  = Q_NULLPTR);
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
    void             init_wdg();
    void             setWidgets(QString);
};

#endif // FILESYSTEMS_H
