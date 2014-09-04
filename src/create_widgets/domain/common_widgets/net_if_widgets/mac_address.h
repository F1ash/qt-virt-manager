#ifndef MAC_ADDRESS_H
#define MAC_ADDRESS_H

#include <QWidget>

class MAC_Address : public QWidget
{
    Q_OBJECT
public:
    explicit MAC_Address(QWidget *parent = 0);

signals:

public slots:
    QString getMACAddress() const;

};

#endif // MAC_ADDRESS_H
