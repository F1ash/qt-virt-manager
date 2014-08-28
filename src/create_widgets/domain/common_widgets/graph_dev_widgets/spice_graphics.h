#ifndef SPICE_GRAPHICS_H
#define SPICE_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"

class Spice_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit Spice_Graphics(QWidget *parent = 0);

private:
    QLabel          *addrLabel;
    QComboBox       *address;
    QCheckBox       *autoPort;
    QSpinBox        *port;
    QCheckBox       *usePassw;
    QLineEdit       *passw;
    QLabel          *keymapLabel;
    QComboBox       *keymap;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void usePort(bool);
    void usePassword(bool);
    void addressEdit(QString);
};

#endif // SPICE_GRAPHICS_H
