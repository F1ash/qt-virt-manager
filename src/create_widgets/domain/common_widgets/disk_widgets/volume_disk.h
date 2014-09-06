#ifndef VOLUME_DISK_H
#define VOLUME_DISK_H

#include "create_widgets/domain/_qwidget.h"

class Volume_Disk : public _QWidget
{
    Q_OBJECT
public:
    explicit Volume_Disk(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;
};

#endif // VOLUME_DISK_H
