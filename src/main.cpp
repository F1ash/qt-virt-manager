#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qt_virt_manager);
    QApplication a(argc, argv);
    QString name("qt-virt-manager");
    a.setOrganizationName(name);
    a.setApplicationName(name);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    MainWindow w;
    w.show();
    return a.exec();
}
