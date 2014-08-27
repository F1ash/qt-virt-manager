#ifndef SOUND_DEVICE_H
#define SOUND_DEVICE_H

#include "create_widgets/domain/_qwidget.h"
#include "device_address.h"
#include <QRadioButton>

class SoundDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit SoundDevice(QWidget *parent = 0);

private:
    QLabel          *modelLabel;
    QComboBox       *model;
    QRadioButton    *defaultICH6reg;
    QRadioButton    *duplexICH6reg;
    QRadioButton    *microICH6reg;
    QHBoxLayout     *regWdgLayout;
    QWidget         *regWdg;
    DeviceAddress   *addr;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument    getDevDocument() const;

private slots:
    void modelChanged(QString);
};

#endif // SOUND_DEVICE_H
