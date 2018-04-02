#include "get_url_dialog.h"
#include <QMessageBox>

URLMenu::URLMenu(QWidget *parent) :
    QMenu(parent)
{
    delURL = addAction(
                QIcon::fromTheme("delete"),
                tr("delete URL from list"));
    clearList = addAction(
                QIcon::fromTheme("edit-clear"),
                tr("clear URL list"));

    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(actionTriggered(QAction*)));
}
Menu_Act URLMenu::getState() const
{
    return state;
}
void URLMenu::actionTriggered(QAction *act)
{
    if ( act==clearList ) {
        state = CLEAR_LIST;
    } else if ( act==delURL ) {
        state = DELETE_ITEM;
    } else {
        state = NONE;
    };
}

GetURLDialog::GetURLDialog(QWidget *parent) :
    QDialog(parent)
{
    url.clear();
    QIcon::setThemeName("QtRemoteViewer");
    setWindowIcon(QIcon::fromTheme("remote-desktop-viewer"));
    setWindowTitle(tr("Get URL"));
    restoreGeometry(settings.value("GetURLGeometry").toByteArray());
    push = new QPushButton(this);
    push->setIcon(QIcon::fromTheme("disconnect"));
    push->setToolTip(tr("Connect"));
    info = new QPushButton(this);
    info->setIcon(QIcon::fromTheme("info"));
    info->setToolTip(tr("Info"));
    urlEdit = new QLineEdit(this);
    urlEdit->setPlaceholderText("protocol://[host[:port]][/?extensions]");
    urlEdit->setToolTip("spice://example.com:5900\nvnc://192.168.0.3:5901\n\
vnc://example.com/?transport=ssh&user=root&addr=192.168.122.1&port=5902\n\
vnc:///?socket=/var/lib/libvirt/qemu/domain-7-myVM/vnc.sock");
    urlLayout = new QHBoxLayout(this);
    urlLayout->addWidget(urlEdit);
    urlLayout->addWidget(push);
    urlLayout->addWidget(info);
    urlWdg = new QWidget(this);
    urlWdg->setLayout(urlLayout);
    urlList = new QListWidget(this);
    urlList->setContextMenuPolicy(Qt::CustomContextMenu);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(urlWdg);
    commonLayout->addWidget(urlList);
    setLayout(commonLayout);
    connect(push, SIGNAL(released()),
            this, SLOT(saveUniqueURL()));
    connect(info, SIGNAL(released()),
            this, SLOT(showInfo()));
    connect(urlEdit, SIGNAL(returnPressed()),
            this, SLOT(saveUniqueURL()));
    connect(urlList, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(urlMenuRequested(const QPoint&)));
    connect(urlList, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(urlActivated(QListWidgetItem*)));

    settings.beginGroup("URLs");
    foreach (QString key, settings.allKeys()) {
        urlList->addItem(settings.value(key).toString());
    };
    settings.endGroup();
}
GetURLDialog::~GetURLDialog()
{
    settings.clear();
    settings.setValue("GetURLGeometry", saveGeometry());
    settings.beginGroup("URLs");
    for (int i=0; i<urlList->count(); i++) {
        QListWidgetItem *item =
                urlList->item(i);
        if ( item==nullptr ) continue;
        settings.setValue(
                    QString::number(i),
                    item->text());
    };
    settings.endGroup();
}

QString GetURLDialog::getURL() const
{
    return url;
}
void GetURLDialog::saveToHistory(const QString &_url)
{
    if ( !_url.isEmpty() && isUnique(_url) )
        urlList->insertItem(0, _url);
}

bool GetURLDialog::isUnique(const QString &_url) const
{
    bool exist = true;
    for (int i=0; i<urlList->count(); i++) {
        QListWidgetItem *item =
                urlList->item(i);
        if ( item==nullptr ) continue;
        if ( item->text()==_url ) {
            exist = false;
        };
    };
    return exist;
}

/* private slots */
void GetURLDialog::closeEvent(QCloseEvent *ev)
{
    ev->accept();
    done(255);
}
void GetURLDialog::saveUniqueURL()
{
    url = urlEdit->text();
    saveToHistory(url);
    done(0);
}
void GetURLDialog::showInfo()
{
    QMessageBox::information(
                this,
                tr("URL schema"),
                tr("If you have SSH access to remote host\n\
and an internal address for VM graphics\n\
then you can use Remote Viewer with such path:\n\
<vnc|spice>://[HOST[:PORT]]/?\n\
[transport=ssh&user=<USER>&]\n\
addr=<IP>&port=<NUMBER>\n\
[&passwd=<PASSWORD>]\n\
\nIf you have graphic socket on local or remote host:\n\
<vnc|spice>://[HOST[:PORT]]/?\n\
[transport=ssh&user=<USER>&]\n\
socket=/path/to/socket\n\
[&passwd=<PASSWORD>]"),
                QMessageBox::Ok);
}
void GetURLDialog::urlMenuRequested(const QPoint &pos)
{
    URLMenu *m = new URLMenu(this);
    m->move(mapToGlobal(pos));
    m->exec();
    const Menu_Act _state = m->getState();
    m->deleteLater();
    if ( _state==DELETE_ITEM ) {
        QModelIndex idx =
                urlList->indexAt(pos);
        QListWidgetItem *item =
                urlList->takeItem(idx.row());
        delete item;
        item = nullptr;
    } else if ( _state==CLEAR_LIST ) {
        url.clear();
        urlList->clear();
    };

}
void GetURLDialog::urlActivated(QListWidgetItem *item)
{
    if ( nullptr!=item ) {
        QString prevURL = urlEdit->text();
        urlEdit->setText(item->text());
        urlList->takeItem(
                    urlList->row(item));
        urlList->removeItemWidget(item);
        if ( !prevURL.isEmpty() && isUnique(prevURL) )
            urlList->insertItem(0, prevURL);
    };
}
