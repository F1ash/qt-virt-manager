#ifndef SEARCH_THREAD_H
#define SEARCH_THREAD_H

#include <QThread>
#include <QStringList>
#include "virt_objects/_virt_thread.h"
#include <QDebug>

class SearchThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit SearchThread(QObject *parent = NULL);

signals:
    void        localConnFound(QString&);

private:
    QStringList URIs;

public slots:
    void        run();
    void        setURIList();
    void        compareURI(QString&);
};

#endif // SEARCH_THREAD_H
