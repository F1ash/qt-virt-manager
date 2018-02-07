#include "vm_viewer/get_url_dialog.h"
#if WITH_SPICE_SUPPORT
#include "vm_viewer/spice/spice_viewer_only.h"
#endif
#if WITH_VNC_SUPPORT
#include "vm_viewer/vnc/vnc_viewer_only.h"
#endif
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qt_remote_viewer);
    QString name("qt-remote-viewer");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QTranslator tr;
    QLocale lc = QLocale();
    if ( lc.language() == QLocale::Russian ) {
        tr.load("remote_viewer_ru");
    };
    QString url;
    bool inLoop = false;
    int _ret = 0;
    while ( _ret!=255 ) {
        QApplication a(argc, argv);
        a.setOrganizationName(name);
        a.setApplicationName(name);
        a.installTranslator(&tr);
        if ( argc>1 && !inLoop ) {
            url = argv[1];
        } else {
            GetURLDialog *d =
                    new GetURLDialog();
            d->saveToHistory(url);
            _ret = d->exec();
            url = d->getURL();
            delete d;
            d = nullptr;
        };
        if ( _ret!=255 ) {
            inLoop = true;
            VM_Viewer_Only *w = nullptr;
            if ( url.startsWith("vnc", Qt::CaseInsensitive) ) {
#if WITH_VNC_SUPPORT
                w = new VNC_Viewer_Only(nullptr, url);
#else
                QMessageBox::information(
                            Q_NULLPTR,
                            "VM Viewer",
                            QString("Application built without VNC"));
#endif
            } else if ( url.startsWith("spice", Qt::CaseInsensitive) ) {
#if WITH_SPICE_SUPPORT
                w = new Spice_Viewer_Only(nullptr, url);
#else
                QMessageBox::information(
                            Q_NULLPTR,
                            "VM Viewer",
                            QString("Application built without SPICE"));
#endif
            };
            if ( w!=nullptr ) {
                // it will be showed when connect to VM will be success
                //w->show();
                a.exec();
            };
        };
    };
    return 0;
}
