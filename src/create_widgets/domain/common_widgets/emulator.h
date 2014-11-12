#ifndef EMULATOR_H
#define EMULATOR_H

#include "create_widgets/domain/_qwidget.h"

class Emulator : public _QWidget
{
    Q_OBJECT
public:
    explicit Emulator(QWidget *parent = NULL);
    QLabel          *emulator;

private:
    QLabel          *emulLabel;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // EMULATOR_H
