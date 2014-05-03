#include "lxc_viewer.h"

LXC_Viewer::LXC_Viewer(QWidget *parent, virConnect *conn, QString str) :
    QWidget(parent), jobConnect(conn), domain(str)
{
    QColor color;
    color.setRgb(0, 0, 0, 255);
    pallete.setColor(QPalette::Base, color);
    color.setRgb(255, 255, 255, 255);
    pallete.setColor(QPalette::Text, color);
    display = new QTextBrowser(this);
    display->setOpenExternalLinks(true);
    display->setUndoRedoEnabled(true);
    display->setOverwriteMode(true);
    display->setPalette(pallete);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(display);
    setLayout(commonLayout);
    if ( jobConnect!=NULL ) {
        domainPtr = getDomainPtr();
    };
    if ( domainPtr!=NULL ) {
        stream = virStreamNew( jobConnect, VIR_STREAM_NONBLOCK );
        if ( stream==NULL ) {
            display->append("Create virtual stream failed...");
        } else {
            /*
             * Older servers did not support either flag,
             * and also did not forbid simultaneous clients on a console,
             * with potentially confusing results.
             * When passing @flags of 0 in order to support a wider range of server versions,
             * it is up to the client to ensure mutual exclusion.
             */
            if ( virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_SAFE)+1 ) {
                display->append("Console opened in SAFE-mode...");
            } else if ( virDomainOpenConsole( domainPtr, NULL, stream, VIR_DOMAIN_CONSOLE_FORCE )+1 ) {
                display->append("Console opened in FORCE-mode...");
            } else if ( virDomainOpenConsole( domainPtr, NULL, stream, 0 )+1 ) {
                display->append("Console opened in ZIRO-mode...");
            } else {
               display->append("Open console failed...");
            };
        };
    } else display->append("Connect or Domain is NULL...");
}
LXC_Viewer::~LXC_Viewer()
{
    delete display;
    display = 0;
    delete commonLayout;
    commonLayout = 0;
    if ( stream!=NULL ) {
        virStreamFinish(stream);
        virStreamFree(stream);
    };
    emit closed();
}

/* public slots */
void LXC_Viewer::close()
{
    this->close();
}

/* private slots */
virDomain* LXC_Viewer::getDomainPtr() const
{
    return virDomainLookupByName(jobConnect, domain.toUtf8().data());
}
