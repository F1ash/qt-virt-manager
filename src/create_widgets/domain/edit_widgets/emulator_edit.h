#ifndef EMULATOR_EDIT_H
#define EMULATOR_EDIT_H

#include "create_widgets/domain/common_widgets/emulator.h"

class Emulator_Edit : public Emulator
{
    Q_OBJECT
public:
    explicit Emulator_Edit(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

signals:

public slots:
    void setDataDescription(QString&);

};

#endif // EMULATOR_EDIT_H
