#include "get_url_dialog.h"
#include <QDebug>

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
    urlEdit = new QLineEdit(this);
    urlEdit->setPlaceholderText("vnc://host:port | spice://host:port");
    urlLayout = new QHBoxLayout(this);
    urlLayout->addWidget(urlEdit);
    urlLayout->addWidget(push);
    urlWdg = new QWidget(this);
    urlWdg->setLayout(urlLayout);
    urlList = new QListWidget(this);
    urlList->setContextMenuPolicy(Qt::CustomContextMenu);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(urlWdg);
    commonLayout->addWidget(urlList);
    setLayout(commonLayout);
    connect(push, SIGNAL(released()),
            this, SLOT(closeDialog()));
    connect(urlEdit, SIGNAL(returnPressed()),
            this, SLOT(closeDialog()));
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
    for (uint i=0; i<urlList->count(); i++) {
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
bool GetURLDialog::isUnique(const QString &) const
{
    bool exist = true;
    for (uint i=0; i<urlList->count(); i++) {
        QListWidgetItem *item =
                urlList->item(i);
        if ( item==nullptr ) continue;
        if ( item->text()==url ) {
            exist = false;
        };
    };
    return exist;
}

/* private slots */
void GetURLDialog::closeDialog()
{
    url = urlEdit->text();
    if ( isUnique(url) )
        urlList->insertItem(0, url);
    done(0);
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
