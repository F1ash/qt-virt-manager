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

signals:
    void                 dataChanged();

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
