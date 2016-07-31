#include "vm_viewer/get_url_dialog.h"
#include "vm_viewer/spice/spice_viewer_only.h"
#include "vm_viewer/vnc/vnc_viewer_only.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qt_remote_viewer);
    QApplication a(argc, argv);
    QString name("qt-remote-viewer");
    a.setOrganizationName(name);
    a.setApplicationName(name);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    bool getURL = true;
    QString url;
    if ( argc>1 ) {
        getURL = false;
        url = argv[1];
    } else {
        GetURLDialog *d =
                new GetURLDialog();
        d->exec();
        url = d->getURL();
        d->deleteLater();
    };
    if ( url.startsWith("vnc") ) {
        VNC_Viewer_Only *w =
                new VNC_Viewer_Only(nullptr, url);
        w->show();
    } else if ( url.startsWith("spice") ) {
        Spice_Viewer_Only *w =
                new Spice_Viewer_Only(nullptr, url);
        w->show();
    } else
        return -1;
    return a.exec();
}
