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
#include <QDebug>

struct MIGR_ARGS {
    int flags           = 0;
    int bandwidth       = 0;
    int maxDownTime     = 0;
    QString uri         = QString();
    QString new_name    = QString();
    QString connName    = QString();
};

class MigrateDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MigrateDialog(QWidget *parent = NULL,
                           const QString arg1 = "-",
                           const QString arg2 = "-",
                           const QString arg3 = "-",
                           const QStringList list = QStringList());
    ~MigrateDialog();

signals:

private:
    QSettings        settings;
    int              exitCode = 0;
    int              m_flags = 0;
    bool             p2p = false;
    MIGR_ARGS        migrateArgs;
    QStringList      connList;
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
    const QString    hostName;
    const QString    connType;
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

public slots:
    MIGR_ARGS        getMigrateArgs() const;

private slots:
    void closeEvent(QCloseEvent *ev);
    void cancelClicked();
    void migrateClicked();
    void advancedVisibility(bool);
    void connectChanged(int);
    void p2pChanged(bool);
    void bandWdthVisibility(bool);
    void maxDownTimeVisibility(bool);
    void migrDiskRegimeChanged();
    void migrTransportChanged();
    void migrTransportChanged(bool);

};

#endif // MIGRATE_DIALOG_H
