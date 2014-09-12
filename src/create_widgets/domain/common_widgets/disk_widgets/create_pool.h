#ifndef CREATE_POOL_H
#define CREATE_POOL_H

#include "_create_storage.h"

class CreatePool : public _CreateStorage
{
    Q_OBJECT
public:
    explicit CreatePool(QWidget *parent = 0);

signals:

public slots:
    QString getStorageXMLDescFileName() const;

private slots:
    void typeChanged(QString);

};

#endif // CREATE_POOL_H
