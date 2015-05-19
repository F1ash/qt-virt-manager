#ifndef SEARCH_THREAD_H
#define SEARCH_THREAD_H

#include <QThread>
#include <QStringList>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <QDebug>

class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(QObject *parent = NULL);
    ~SearchThread();

signals:
    void        localConnFound(QString&);

private:
    QStringList URIs;

public slots:
    void        run();
    void        compareURI(QString&);
};

#endif // SEARCH_THREAD_H
