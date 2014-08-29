#ifndef SPICE_GRAPHICS_H
#define SPICE_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"

class Spice_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit Spice_Graphics(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

signals:
    void errorMsg(QString);

private:
    QLabel          *addrLabel;
    QComboBox       *address;
    QComboBox       *networks;
    QCheckBox       *autoPort;
    QSpinBox        *port;
    QCheckBox       *tlsPortLabel;
    QSpinBox        *tlsPort;
    QCheckBox       *usePassw;
    QLineEdit       *passw;
    QLabel          *keymapLabel;
    QComboBox       *keymap;
    QLabel          *defaultLabel;
    QComboBox       *defaultPolicy;
    QCheckBox       *mainLabel, *displayLabel, *inputsLabel, *cursorLabel,
                    *playbackLabel, *recordLabel, *smartcardLabel, *usbredirLabel;
    QComboBox       *main, *display, *inputs, *cursor, *playback,
                    *record, *smartcard, *usbredir;
    QGridLayout     *elementsLayout;
    QWidget         *elements;
    QGridLayout     *commonLayout;

    QStringList      nets;
    virErrorPtr      virtErrors;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void useAutoPort(bool);
    void useTLSPort(bool);
    void usePassword(bool);
    void addressEdit(QString);
    void elementsSetRequired(bool);
    void defaultPolicyChanged(int);
    void readNetworkList();

    void sendConnErrors();
    void sendGlobalErrors();
};

#endif // SPICE_GRAPHICS_H
