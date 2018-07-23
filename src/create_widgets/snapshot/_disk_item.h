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
    explicit _DiskItem(QWidget *parent = Q_NULLPTR);

signals:

private:
    QCheckBox           *usage;
    QLineEdit           *name;
    QLineEdit           *source;
    QComboBox           *driver;
    QComboBox           *snapshotType;
    QHBoxLayout         *paramLayout;
    QWidget             *params;
    QVBoxLayout         *baseLayout;
    QWidget             *baseWdg;
    QHBoxLayout         *commonLayout;

public slots:
    void                 setDiskName(QString&);
    void                 setDriverType(QString&);
    void                 setSnapshotType(QString&);
    QString              getName() const;
    QString              getSource() const;
    QString              getDriverType() const;
    QString              getSnapshotType() const;
    bool                 isUsed() const;

private slots:
    void                 snapshotTypeChanged(QString);
    void                 driverTypeChanged(QString);
};

#endif // _DISK_ITEM_H
