#include "mainwindow.h"
#include <QApplication>

#include <sys/stat.h>

int main(int argc, char *argv[])
{
    /*
     * WARNING:
     * for work askpass need to run
     * application as daemon
     */
    /*
    pid_t pid, sid;

    //Fork the Parent Process
    //because ssh-askpass try connect to console
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    */

    // init GUI
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
