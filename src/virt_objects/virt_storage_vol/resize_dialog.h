#ifndef RESIZE_DIALOG_H
#define RESIZE_DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "virt_objects/_virt_thread.h"

class resizeHelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit resizeHelperThread(
            QObject        *pnullptrt   = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr,
            QString         _poolName   = QString(),
            QString         _volName    = QString());
    unsigned long long  _size;
    QString             poolName, volName;
    bool                result;
    void                run();
};

class ResizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResizeDialog(
            QWidget        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr,
            QString         _poolName   = QString(),
            QString         _volName    = QString());

private:
    const unsigned long long   bytes = 1;
    const unsigned long long   KiB   = 1024;
    const unsigned long long   MiB   = 1048576;
    const unsigned long long   GiB   = 1073741824;
    const unsigned long long   TiB   = 1099511627776;
    unsigned long long   _size, currentRange;
    QLabel              *sizeLabel;
    QSpinBox            *size;

    QPushButton         *ok;
    QPushButton         *cancel;

    QHBoxLayout         *buttonLayout;
    QWidget             *buttons;
    QVBoxLayout         *commonlayout;

    resizeHelperThread  *hlpThread;

private slots:
    void okClicked();
    void cancelClicked();
    void changeRange(int);
    void changeRangeLong(unsigned long long);
    void setCurrentSize();

public slots:
    unsigned long long getNewSize() const;

};

#endif // RESIZE_DIALOG_H
