#ifndef _DISKITEM_H
#define _DISKITEM_H

#include <QWidget>
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
    QHBoxLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

public slots:
};

#endif // _DISKITEM_H
