#ifndef EMULATOR_H
#define EMULATOR_H

#include "create_widgets/common/_qwidget.h"

class Emulator : public _QWidget
{
    Q_OBJECT
public:
    explicit Emulator(QWidget *parent = nullptr);
    QLabel          *emulator;

private:
    QLabel          *emulLabel;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // EMULATOR_H
