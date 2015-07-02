#ifndef SECRET_CONTROL_THREAD_H
#define SECRET_CONTROL_THREAD_H

#include "virt_objects/control_thread.h"

class SecretControlThread : public ControlThread
{
    Q_OBJECT
public:
    explicit SecretControlThread(QObject *parent = NULL);

signals:

private:

public slots:
    void         execAction(uint, TASK);

private slots:
    void         run();
    Result       getAllSecretList();
    Result       defineSecret();
    Result       undefineSecret();
    Result       getVirtSecretXMLDesc();
};

#endif // SECRET_CONTROL_THREAD_H
