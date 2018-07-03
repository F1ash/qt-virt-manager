#ifndef _DISKS_H
#define _DISKS_H

#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include <QDomDocument>
#include "_disk_item.h"
//#include <QDebug>

class _Disks : public QScrollArea
{
    Q_OBJECT
public:
    explicit _Disks(QWidget *parent = nullptr);

private:
    QWidget         *scrolled;
    QVBoxLayout     *disksLayout;

public slots:
    void             setDisksData(QDomElement&, bool);
    QDomDocument     getElements(bool) const;
    void             addStretch();
};

#endif // _DISKS_H
