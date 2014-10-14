#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>

class MAC_Address : public QWidget
{
    Q_OBJECT
public:
    explicit MAC_Address(QWidget *parent = NULL);

private:
    QCheckBox           *useMac;
    QLineEdit           *mac;
    QGridLayout         *commonLayout;

public slots:
    QString getMACAddress() const;
};

#endif // MAC_ADDRESS_H
