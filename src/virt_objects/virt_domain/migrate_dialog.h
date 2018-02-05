#ifndef MIGRATE_DIALOG_H
#define MIGRATE_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextBrowser>
#include <QSettings>
#include <QScrollArea>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "virt_objects/_virt_thread.h"
//#include <QDebug>

class _HELP : public QObject
{
    Q_OBJECT
public:

    static QString APPEND_URI_HELP_0() {
        return tr("The destination libvirtd server \
will automatically determine the native hypervisor URI for migration, \
based off the primary hostname. ");
    }

    static QString APPEND_URI_HELP_1() {
        return tr("The optional uri parameter controls \
how the source libvirtd connects to the destination libvirtd, \
in case it is not accessible using the same address that the client uses \
to connect to the destination, or a different encryption/auth scheme is required. ");
    }

    static QString APPEND_URI_HELP_2() {
        return tr("The native hypervisor URI format is not used at all. ");
    }

    static QString APPEND_URI_HELP_3() {
        return tr("To force migration over an alternate \
network interface the optional hypervisor specific URI must be provided. ");
    }

    static QString APPEND_URI_HELP_4() {
        return tr("There is no scope for forcing \
an alternative network interface for the native migration data with this method. ");
    }

    static QString APPEND_URI_HELP_5() {
        return tr("There is no use or requirement for a destination \
libvirtd instance at all. This is typically used when the hypervisor has \
its own native management daemon available to handle incoming migration attempts \
on the destination. ");
    }

    static QString APPEND_URI_HELP_6() {
        return tr("The destination URI must be reachable using \
the source libvirtd credentials (which are not necessarily the same as \
the credentials of the client in connecting to the source). ");
    }

    static QString EXAMPLE_URI_1() {
        return tr("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME DEST-LIBVIRT-URI [HV-URI]<br><br>\
<i>eg using default network interface</i>:<br><br>\
virsh migrate web1 qemu+ssh://desthost/system<br>\
virsh migrate web1 xen+tls://desthost/system<br><br>\
<i>eg using secondary network interface:</i><br><br>\
virsh migrate web1 qemu://desthost/system tcp://10.0.0.1/<br>\
virsh migrate web1 xen+tcp://desthost/system xenmigr:10.0.0.1/");
    }

    static QString EXAMPLE_URI_2() {
        return tr("<b>syntax <font color='red'>for virsh</font></b>:<br>\
This mode cannot be invoked from virsh ");
    }

    static QString EXAMPLE_URI_3() {
        return tr("<b>syntax <font color='red'>for virsh</font></b>:<br>\
This mode cannot be invoked from virsh ");
    }

    static QString EXAMPLE_URI_4() {
        return tr("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME HV-URI<br><br>\
<i>eg using same libvirt URI for all connections</i>:<br><br>\
virsh migrate --direct web1 xenmigr://desthost/");
    }

    static QString EXAMPLE_URI_5() {
        return tr("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME DEST-LIBVIRT-URI [ALT-DEST-LIBVIRT-URI]<br><br>\
<i>eg using same libvirt URI for all connections</i>:<br><br>\
virsh migrate --p2p web1 qemu+ssh://desthost/system<br><br>\
<i>eg using different libvirt URI auth scheme for peer2peer connections</i>:<br><br>\
virsh migrate --p2p web1 qemu+ssh://desthost/system qemu+tls:/desthost/system<br><br>\
<i>eg using different libvirt URI hostname for peer2peer connections</i>:<br><br>\
virsh migrate --p2p web1 qemu+ssh://desthost/system qemu+ssh://10.0.0.1/system");
    }

    static QString EXAMPLE_URI_6() {
        return tr("<b>syntax <font color='red'>for virsh</font></b>:<br>\
virsh migrate GUESTNAME DEST-LIBVIRT-URI [ALT-DEST-LIBVIRT-URI]<br><br>\
<i>eg using same libvirt URI for all connections</i>:<br><br>\
virsh migrate --p2p --tunnelled web1 qemu+ssh://desthost/system<br><br>\
<i>eg using different libvirt URI auth scheme for peer2peer connections</i>:<br><br>\
virsh migrate --p2p --tunnelled web1 qemu+ssh://desthost/system qemu+tls:/desthost/system<br><br>\
<i>eg using different libvirt URI hostname for peer2peer connections</i>:<br><br>\
virsh migrate --p2p --tunnelled web1 qemu+ssh://desthost/system qemu+ssh://10.0.0.1/system");
    }

};

struct MIGR_ARGS {
    int flags           = 0;
    int bandwidth       = 0;
    int maxDownTime     = 0;
    QString uri         = QString();
    QString new_name    = QString();
    QString connName    = QString();
};

class mgrHelpThread : public _VirtThread
{
public:
    explicit mgrHelpThread(
            QObject         *parent     = nullptr,
            virConnectPtr   *connPtrPtr = nullptr);
    QString hostName, connType;
    void    run();
};

class MigrateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MigrateDialog(
            QWidget         *parent      = nullptr,
            QString          _domain     = QString(),
            virConnectPtr   *connPtrPtr  = nullptr);

signals:

private:
    QSettings        settings;
    int              m_flags = 0;
    bool             p2p = false;
    MIGR_ARGS        migrateArgs;
    QComboBox       *connectList;
    QWidget         *advanced;
    QSplitter       *splitter;
    QWidget         *connectivity;
    QScrollArea     *flags;
    QWidget         *buttons;
    QPushButton     *ok;
    QPushButton     *cancel;
    QCheckBox       *useAdvanced;

    const QString    domainName;
    QLineEdit       *Name;
    QLabel          *host;
    QLabel          *newHost;

    QLabel          *uriLabel;
    QLineEdit       *uri;
    QLabel          *help;
    QTextBrowser    *helpText;
    QLabel          *helpLinkLabel;
    QGridLayout     *connLayout;

    QRadioButton    *nativeMigration;
    QRadioButton    *tunnelMigration;
    QLabel          *p2pMigration;
    QLabel          *maxDownTimeLabel;
    QCheckBox       *maxDownCheck;
    QSpinBox        *maxDownTime;
    QLabel          *bandWdthLabel;
    QCheckBox       *bandWdthCheck;
    QCheckBox       *liveMigration;
    QCheckBox       *p2pCheck;
    QCheckBox       *persistDestMigration;
    QCheckBox       *undefineSourceMigration;
    QCheckBox       *pausedMigration;
    QCheckBox       *fullNonSharedDiskMigration;
    QCheckBox       *incNonSharedDiskMigration;
    QCheckBox       *unsafeMigration;
    QCheckBox       *offlineMigration;
    QCheckBox       *compressedMigration;
    QCheckBox       *abortOnMigration;
    QSpinBox        *bandwidth;
    QGridLayout     *advLayout;

    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    mgrHelpThread   *hlpThread;

public slots:
    MIGR_ARGS        getMigrateArgs() const;

private slots:
    void             fillData();
    void             closeEvent(QCloseEvent*);
    void             cancelClicked();
    void             migrateClicked();
    void             advancedVisibility(bool);
    void             connectChanged(int);
    void             p2pChanged(bool);
    void             bandWdthVisibility(bool);
    void             maxDownTimeVisibility(bool);
    void             migrDiskRegimeChanged();
    void             migrTransportChanged();
    void             migrTransportChanged(bool);

};

#endif // MIGRATE_DIALOG_H
