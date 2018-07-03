#ifndef SEARCH_THREAD_H
#define SEARCH_THREAD_H

#include <QStringList>
#include "virt_objects/_virt_thread.h"
//#include <QDebug>

class SearchThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit SearchThread(QObject *parent = nullptr);

signals:
    void        localConnFound(const QString&);

private:
    QStringList URIs;

public slots:
    void        run();
    void        setURIList();
    void        compareURI(const QString&);
};

#endif // SEARCH_THREAD_H
