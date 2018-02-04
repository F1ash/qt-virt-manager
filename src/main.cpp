#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qt_virt_manager);
    QApplication a(argc, argv);
    QString name("qt-virt-manager");
    a.setOrganizationName(name);
    a.setApplicationName(name);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QTranslator tr;
    QLocale lc = QLocale();
    if ( lc.language() == QLocale::Russian ) {
        tr.load("qt_virt_manager_ru");
    };
    a.installTranslator(&tr);
    MainWindow w;
    w.show();
    return a.exec();
}
