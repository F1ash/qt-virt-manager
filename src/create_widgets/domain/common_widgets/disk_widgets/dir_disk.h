#ifndef DIR_DISK_H
#define DIR_DISK_H

#include <QFileDialog>
#include <QPushButton>
#include "create_widgets/domain/_qwidget.h"
#include "_target.h"
#include "_device.h"
#include "_read_only.h"
#include "_startup_policy.h"

class Dir_Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit Dir_Disk(
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
    void setDirPath();
};

#endif // DIR_DISK_H
