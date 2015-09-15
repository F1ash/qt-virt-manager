#ifndef SPICE_GRAPHICS_H
#define SPICE_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"

class Spice_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit Spice_Graphics(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

signals:
    void             errorMsg(QString);

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
    QGridLayout     *baseLayout;
    QWidget         *baseElements;
    QCheckBox       *mainLabel, *displayLabel, *inputsLabel, *cursorLabel,
                    *playbackLabel, *recordLabel, *smartcardLabel, *usbredirLabel;
    QComboBox       *main, *display, *inputs, *cursor, *playback,
                    *record, *smartcard, *usbredir;
    QGridLayout     *policyElementsLayout;
    QWidget         *policyElements;
    QCheckBox       *compress, *compressImage, *compressJpeg,
                    *compressZlib, *compressPlayback;
    QComboBox       *imageElement, *jpegElement,
                    *zlibElement, *playbackElement;
    QGridLayout     *compressElementsLayout;
    QWidget         *compressElements;
    QCheckBox       *addition, *streaming, *clipboard,
                    *mouse, *filetransfer;
    QComboBox       *streamingElement, *clipboardElement,
                    *mouseElement, *filetransferElement;
    QGridLayout     *additionElementsLayout;
    QWidget         *additionElements;
    QVBoxLayout     *commonLayout;

    QStringList      nets;
    virErrorPtr      virtErrors;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             useAutoPort(bool);
    void             useTLSPort(bool);
    void             usePassword(bool);
    void             addressEdit(QString);
    void             policyElementsSetRequired(bool);
    void             defaultPolicyChanged(int);
    void             compressStateChanged(bool);
    void             additionStateChanged(bool);
    void             readNetworkList();

    void             sendConnErrors();
    void             sendGlobalErrors();
};

#endif // SPICE_GRAPHICS_H
