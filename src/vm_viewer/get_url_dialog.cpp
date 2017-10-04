#include "get_url_dialog.h"
#include <QMessageBox>

URLMenu::URLMenu(QWidget *parent) :
    QMenu(parent)
{
    state = false;
    delURL = addAction(
                QIcon::fromTheme("delete"),
                "delete URL from list");
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(actionTriggered(QAction*)));
}
bool URLMenu::isActivated() const
{
    return state;
}
void URLMenu::actionTriggered(QAction *act)
{
    state = (act==delURL);
}

GetURLDialog::GetURLDialog(QWidget *parent) :
    QDialog(parent)
{
    url.clear();
    QIcon::setThemeName("QtRemoteViewer");
    setWindowIcon(QIcon::fromTheme("remote-desktop-viewer"));
    setWindowTitle("Get URL");
    restoreGeometry(settings.value("GetURLGeometry").toByteArray());
    push = new QPushButton(this);
    push->setIcon(QIcon::fromTheme("disconnect"));
    push->setToolTip("Connect");
    info = new QPushButton(this);
    info->setIcon(QIcon::fromTheme("info"));
    info->setToolTip("Info");
    urlEdit = new QLineEdit(this);
    urlEdit->setPlaceholderText("protocol://host:port");
    urlEdit->setToolTip("spice://example.com:5900\nvnc://192.168.0.3:5901\n\
vnc://example.com/?transport=ssh&user=root&addr=192.168.122.1&port=5902");
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
                "For VM on remote host",
                "If you have SSH access to remote host\n\
and an internal address for VM graphics\n\
then you can use Remote Viewer with such path:\n\
<vnc|spice>://HOST[:PORT]/?transport=ssh&user=<USER>&addr=<IP>&port=<NUMBER>",
                QMessageBox::Ok);
}
void GetURLDialog::urlMenuRequested(const QPoint &pos)
{
    URLMenu *m = new URLMenu(this);
    m->move(mapToGlobal(pos));
    m->exec();
    const bool isActivated = m->isActivated();
    m->deleteLater();
    if ( isActivated ) {
        QModelIndex idx =
                urlList->indexAt(pos);
        QListWidgetItem *item =
                urlList->takeItem(idx.row());
        delete item;
        item = nullptr;
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
