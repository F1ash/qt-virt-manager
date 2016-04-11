#ifndef _QWIDGET_THREADED_H
#define _QWIDGET_THREADED_H

#include "_qwidget.h"
#include "virt_objects/_virt_thread.h"

class qwdHelpThread : public _VirtThread
{
public:
    explicit qwdHelpThread(
            QObject         *parent     = nullptr,
            virConnectPtr   *connPtrPtr = nullptr);
    QString         connType;
    virtual void    run();
};

class _QWidget_Threaded : public _QWidget
{
    Q_OBJECT
public:
    explicit _QWidget_Threaded(
            QWidget         *parent     = nullptr,
            virConnectPtr*   connPtrPtr = nullptr,
            virDomainPtr     domain     = nullptr);
    qwdHelpThread   *hlpThread;

public slots:
    virtual void     init_wdg();
};

#endif // _QWIDGET_THREADED_H
