#ifndef EVENTS_H
#define EVENTS_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include "create_widgets/domain/_changed.h"

class Events : public _Changed
{
    Q_OBJECT
public:
    explicit Events(QWidget *parent = nullptr);
    QCheckBox       *powerOffLabel, *rebootLabel,
                    *crashLabel, *lockFailureLabel;
    QComboBox       *on_powerOff, *on_reboot,
                    *on_crash, *on_lockfailure;

signals:
    //void             dataChanged();

private:
    QGridLayout     *commonLayout;
};

#endif // EVENTS_H
