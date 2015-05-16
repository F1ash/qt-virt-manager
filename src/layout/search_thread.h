#ifndef SEARCH_THREAD_H
#define SEARCH_THREAD_H

#include <QThread>

class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(QObject *parent = NULL);
    ~SearchThread();

signals:
    void        localConnFound(QString&);

public slots:
    void        run();
};

#endif // SEARCH_THREAD_H
