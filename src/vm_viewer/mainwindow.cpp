#include "mainwindow.h"
#include "vm_viewer/spice/spice_viewer_only.h"
#include "vm_viewer/vnc/vnc_viewer_only.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, bool getURL) :
    QMainWindow(parent)
{
    setWindowTitle("Qt Remote Desktop Viewer");
    QIcon::setThemeName("QtRemoteViewer");
    setWindowIcon(QIcon::fromTheme("remote-desktop-viewer"));
    //restoreGeometry(settings.value("Geometry").toByteArray());

    connect(this, SIGNAL(urlGiven(const QString&)),
            this, SLOT(connectToURL(const QString&)));

    if (getURL) showGetURLDialog();
}
MainWindow::~MainWindow()
{
    //settings.setValue("Geometry", saveGeometry());
}

/* private slots */
void MainWindow::showGetURLDialog()
{
    GetURLDialog *d = new GetURLDialog(this);
    d->exec();
    const QString url = d->getURL();
    d->deleteLater();
    emit urlGiven(url);
}

/* public slots */
void MainWindow::connectToURL(const QString &url)
{
    if ( url.isEmpty() ) close();
    if ( url.startsWith("vnc") ) {
        setCentralWidget(new VNC_Viewer_Only(nullptr, url));
    } else if ( url.startsWith("spice") ) {
        setCentralWidget(new Spice_Viewer_Only(nullptr, url));
    } else
        close();
}
