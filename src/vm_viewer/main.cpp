#include "vm_viewer/get_url_dialog.h"
#include "vm_viewer/spice/spice_viewer_only.h"
#include "vm_viewer/vnc/vnc_viewer_only.h"
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
                w = new VNC_Viewer_Only(nullptr, url);
            } else if ( url.startsWith("spice", Qt::CaseInsensitive) ) {
                w = new Spice_Viewer_Only(nullptr, url);
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
