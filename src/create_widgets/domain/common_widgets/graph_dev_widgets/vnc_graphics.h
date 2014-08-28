#ifndef VNC_GRAPHICS_H
#define VNC_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"

class VNC_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit VNC_Graphics(
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
    QCheckBox       *usePassw;
    QLineEdit       *passw;
    QLabel          *keymapLabel;
    QComboBox       *keymap;
    QLabel          *shareLabel;
    QComboBox       *sharePolicy;
    QGridLayout     *commonLayout;

    QStringList      nets;
    virErrorPtr      virtErrors;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void usePort(bool);
    void usePassword(bool);
    void addressEdit(QString);
    void readNetworkList();

    void sendConnErrors();
    void sendGlobalErrors();
};

#endif // VNC_GRAPHICS_H
