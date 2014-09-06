#ifndef FILE_DISK_H
#define FILE_DISK_H

#include <QFileDialog>
#include <QPushButton>
#include "create_widgets/domain/_qwidget.h"
#include "_target.h"
#include "_device.h"
#include "_read_only.h"
#include "_startup_policy.h"

class File_Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit File_Disk(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QPushButton     *browse;
    QLineEdit       *path;
    _StartupPolicy  *startupPolicy;
    _Device         *devType;
    _Target         *target;
    _ReadOnly       *readOnly;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void setFilePath();
};

#endif // FILE_DISK_H
