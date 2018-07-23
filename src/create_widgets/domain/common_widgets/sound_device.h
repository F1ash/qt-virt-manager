#ifndef SOUND_DEVICE_H
#define SOUND_DEVICE_H

#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/device_address.h"
#include <QRadioButton>

class SoundDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit SoundDevice(QWidget *parent = Q_NULLPTR);
    QComboBox       *model;
    QRadioButton    *defaultICH6reg;
    QRadioButton    *duplexICH6reg;
    QRadioButton    *microICH6reg;
    DeviceAddress   *addr;

private:
    QLabel          *modelLabel;
    QHBoxLayout     *regWdgLayout;
    QWidget         *regWdg;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             modelChanged(QString);
};

#endif // SOUND_DEVICE_H
