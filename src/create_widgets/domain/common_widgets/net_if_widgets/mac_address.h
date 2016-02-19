#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include "create_widgets/domain/_changed.h"

class MAC_Address : public _Changed
{
    Q_OBJECT
public:
    explicit MAC_Address(QWidget *parent = nullptr);

signals:
    //void                 dataChanged();

private:
    QCheckBox           *useMac;
    QLineEdit           *mac;
    QGridLayout         *commonLayout;

public slots:
    bool                 isUsed() const;
    void                 setUsage(bool);
    QString              getMACAddress() const;
    void                 setMACAddress(QString&);
};

#endif // MAC_ADDRESS_H
