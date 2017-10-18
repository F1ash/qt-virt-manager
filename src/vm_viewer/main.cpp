#include "vm_viewer/get_url_dialog.h"
#ifndef ONLY_VNC_BUILD
#include "vm_viewer/spice/spice_viewer_only.h"
#endif
#ifndef ONLY_SPICE_BUILD
#include "vm_viewer/vnc/vnc_viewer_only.h"
#endif
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qt_remote_viewer);
    QString name("qt-remote-viewer");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QString url;
    bool inLoop = false;
    int _ret = 0;
    while ( _ret!=255 ) {
        QApplication a(argc, argv);
        a.setOrganizationName(name);
        a.setApplicationName(name);
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
#ifndef ONLY_SPICE_BUILD
                w = new VNC_Viewer_Only(nullptr, url);
#else
                QMessageBox::information(
                            Q_NULLPTR,
                            "VM Viewer",
                            QString("Application built without VNC"));
#endif
            } else if ( url.startsWith("spice", Qt::CaseInsensitive) ) {
#ifndef ONLY_VNC_BUILD
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
