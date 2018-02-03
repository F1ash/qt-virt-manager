#ifndef SPICE_GRAPHICS_H
#define SPICE_GRAPHICS_H

#include "create_widgets/common/_qwidget.h"
#include "virt_objects/_virt_thread.h"

class spice_graphHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit spice_graphHlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    void             run();
signals:
    void             result(QStringList&);
};

class Spice_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit Spice_Graphics(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

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
    QLabel          *keymapLabel, *channelLabel;
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
    spice_graphHlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             useAutoPort(bool);
    void             useTLSPort(bool);
    void             usePassword(bool);
    void             addressEdit(int);
    void             policyElementsSetRequired(bool);
    void             defaultPolicyChanged(int);
    void             compressStateChanged(bool);
    void             additionStateChanged(bool);
    void             readNetworkList(QStringList&);
    void             emitCompleteSignal();
};

#endif // SPICE_GRAPHICS_H
