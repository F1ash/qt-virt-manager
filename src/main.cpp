#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString name = QString("qt-virt-manager");
    a.setOrganizationName(name);
    a.setApplicationName(name);
    MainWindow w;
    w.show();
    
    return a.exec();
}
