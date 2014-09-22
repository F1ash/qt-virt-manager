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
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *sourceLabel;
    QComboBox       *source;
    QHBoxLayout     *sourceLayout;
    QWidget         *sourceWdg;
    QStackedWidget  *info;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // DISK_H
