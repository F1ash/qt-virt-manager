#ifndef CHANNEL_DEVICE_H
#define CHANNEL_DEVICE_H

#include "char_device.h"

class ChannelDevice : public CharDevice
{
    Q_OBJECT
public:
    explicit ChannelDevice(QWidget *parent = 0);
    QComboBox       *chanType;

signals:

private:

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             chanNameChanged(const QString&);

};

#endif // CHANNEL_DEVICE_H
