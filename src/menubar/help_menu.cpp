#include "help_menu.h"
#include <QMessageBox>
#include <QApplication>
#include "donate_dialog.h"
#include "version.h"

HelpMenu::HelpMenu(QWidget *parent) :
    QMenu(parent)
{
    aboutQt = addAction(
                QIcon::fromTheme("qt_icon"),
                tr("About Qt"));
    aboutSelf = addAction(
                QIcon::fromTheme("virtual-engineering"),
                tr("About QtVirtManager"));
    addSeparator();
    donate = addAction(
                QIcon::fromTheme("donate"),
                tr("Donate"));
    connect(aboutQt, SIGNAL(triggered(bool)),
            this, SLOT(showAboutQt()));
    connect(aboutSelf, SIGNAL(triggered(bool)),
            this, SLOT(showAboutSelf()));
    connect(donate, SIGNAL(triggered(bool)),
            this, SLOT(showDonateDialog()));
}

/* private slots */
void HelpMenu::showAboutQt()
{
    QApplication::aboutQt();
}
void HelpMenu::showAboutSelf()
{
    QString message = tr(
                "<a href='http://f1ash.github.io/qt-virt-manager'>\
                 Qt Virtual machines manager</a>, version %1.\
                 <br>Based on Qt %2. Uses libvirt (%3) API.\
                 <br>Implemented graphical consoles\
                 <br>for Virtual Machine displays\
                 <br>(by SPICE/VNC client) and LXC terminals.")
                .arg(QString("%1.%2.%3")
                     .arg(VERSION_MAJOR)
                     .arg(VERSION_MINOR)
                     .arg(VERSION_MICRO))
                .arg(QT_VERSION_STR)
                .arg(libvirtVer);
    QMessageBox::about(
                this,
                tr("About QtVirtManager"),
                message);
}
void HelpMenu::showDonateDialog()
{
    Donate_Dialog *dd = new Donate_Dialog(this);
    dd->exec();
    dd->deleteLater();
}

/* public slots */
void HelpMenu::setLibvirtVersion(const QString &ver)
{
    libvirtVer = ver;
}
