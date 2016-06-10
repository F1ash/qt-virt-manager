#ifndef HELP_MENU_H
#define HELP_MENU_H

#include <QMenu>

class HelpMenu : public QMenu
{
    Q_OBJECT
public:
    explicit HelpMenu(QWidget *parent = nullptr);
    QAction         *aboutQt;
    QAction         *aboutSelf;
    QAction         *donate;

    void             setLibvirtVersion(const QString&);

private:
    QString          libvirtVer;

private slots:
    void             showAboutQt();
    void             showAboutSelf();
    void             showDonateDialog();
};

#endif // HELP_MENU_H
