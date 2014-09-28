#ifndef EMULATOR_EDIT_H
#define EMULATOR_EDIT_H

#include "create_widgets/domain/common_widgets/emulator.h"

class Emulator_Edit : public Emulator
{
    Q_OBJECT
public:
    explicit Emulator_Edit(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

signals:

public slots:
    void setDeviceData(QString&);

};

#endif // EMULATOR_EDIT_H
