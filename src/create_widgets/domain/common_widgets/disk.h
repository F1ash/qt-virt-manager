#ifndef DISK_H
#define DISK_H

#include <QStackedWidget>
#include "disk_widgets/block_disk.h"
#include "disk_widgets/dir_disk.h"
#include "disk_widgets/file_disk.h"
#include "disk_widgets/network_disk.h"
#include "disk_widgets/volume_disk.h"

class Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit Disk(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtr = NULL);
    QComboBox       *source;
    QStackedWidget  *info;

private:
    QLabel          *sourceLabel;
    QHBoxLayout     *sourceLayout;
    QWidget         *sourceWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // DISK_H
