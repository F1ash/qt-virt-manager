#ifndef SMARTCARD_DEVICE_H
#define SMARTCARD_DEVICE_H

#include "create_widgets/domain/_qwidget.h"

class SmartCardDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit SmartCardDevice(QWidget *parent = 0);
    ~SmartCardDevice();

signals:

private:
    QComboBox       *mode;
    QVBoxLayout     *commonLayout;

public slots:
    QDomNodeList getNodeList() const;
    QString      getDevType() const;
    QString      getDevMode() const;
};

#endif // SMARTCARD_DEVICE_H
