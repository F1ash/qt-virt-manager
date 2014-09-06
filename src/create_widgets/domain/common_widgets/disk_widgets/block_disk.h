#ifndef BLOCK_DISK_H
#define BLOCK_DISK_H

#include <QFileDialog>
#include <QPushButton>
#include "create_widgets/domain/_qwidget.h"
#include "_target.h"
#include "_device.h"
#include "_read_only.h"
#include "_startup_policy.h"

class Block_Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit Block_Disk(
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
    void setDevPath();
};

#endif // BLOCK_DISK_H
