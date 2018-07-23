#ifndef CREATE_POOL_H
#define CREATE_POOL_H

#include "_create_storage_widgets/_create_storage.h"

class CreatePool : public _CreateStorage
{
    Q_OBJECT
public:
    explicit CreatePool(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR,
            QString          _connName  = QString(),
            QString          _xmlFile   = QString());
    void             setDataDescription(const QString&);

signals:

private:
    QLabel          *uuidLabel;
    QLineEdit       *uuid;
    QHBoxLayout     *uuidLayout;
    QWidget         *uuidWdg;

public slots:
    int              getResult() const;
    QString          getXMLDescFileName() const;
};

#endif // CREATE_POOL_H
