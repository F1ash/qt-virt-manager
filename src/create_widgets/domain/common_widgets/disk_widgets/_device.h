#ifndef _DEVICE_H
#define _DEVICE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include "create_widgets/domain/_changed.h"

class _Device : public _Changed
{
    Q_OBJECT
public:
    explicit _Device(QWidget *parent = NULL);
    QComboBox   *devType;

signals:
    //void         dataChanged();

private:
    QLabel      *devTypeLabel;
    QHBoxLayout *commonLayout;

public slots:
    QString      getDeviceType() const;
    void         setDeviceType(QString&);
};

#endif // _DEVICE_H
