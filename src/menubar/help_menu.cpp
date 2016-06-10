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
                "About Qt");
    aboutSelf = addAction(
                QIcon::fromTheme("virtual-engineering"),
                "About QtVirtViewer");
    addSeparator();
    donate = addAction(
                QIcon::fromTheme("donate"),
                "Donate");
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
    QString message = QString(
                "Qt Virtual machine manager,\
                \nversion %1.\
                \nBased on Qt %2.\
                \nUsed libvirt (%3) API.\
                \nImplemented graphical consoles for\
                \nVirtual Machine displays\
                \n(by SPICE/VNC client)\
                \nand LXC terminals.")
                .arg(QString("%1.%2.%3")
                     .arg(VERSION_MAJOR)
                     .arg(VERSION_MIDDLE)
                     .arg(VERSION_MINOR))
                .arg(QT_VERSION_STR)
                .arg(libvirtVer);
    QMessageBox::about(
                this,
                "About QtVirtManager",
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
