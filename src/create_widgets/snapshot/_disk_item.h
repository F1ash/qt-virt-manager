#ifndef _DISK_ITEM_H
#define _DISK_ITEM_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

class _DiskItem : public QWidget
{
    Q_OBJECT
public:
    explicit _DiskItem(QWidget *parent = 0);
    ~_DiskItem();

signals:

private:
    QCheckBox           *usage;
    QLineEdit           *name;
    QLineEdit           *source;
    QComboBox           *driver;
    QComboBox           *snapshotType;
    QVBoxLayout         *baseLayout;
    QWidget             *baseWdg;
    QHBoxLayout         *commonLayout;

public slots:
    void                 setDiskName(QString&);
};

#endif // _DISK_ITEM_H
