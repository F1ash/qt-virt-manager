#ifndef EMULATOR_H
#define EMULATOR_H

#include "create_widgets/domain/_qwidget.h"

class Emulator : public _QWidget
{
    Q_OBJECT
public:
    explicit Emulator(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);
    QComboBox       *emulatores;

private:
    QLabel          *hostArch, *emulLabel,
                    *guestEmul;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             setAvailableEmulatores();
    void             guestEmulChanged(QString);
};

#endif // EMULATOR_H
