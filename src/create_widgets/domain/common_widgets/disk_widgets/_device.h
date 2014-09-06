#ifndef _DEVICE_H
#define _DEVICE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>

class _Device : public QWidget
{
    Q_OBJECT
public:
    explicit _Device(QWidget *parent = 0);
    QComboBox   *devType;

private:
    QLabel      *devTypeLabel;
    QHBoxLayout *commonLayout;

public slots:
    QString getDeviceType() const;
};

#endif // _DEVICE_H
