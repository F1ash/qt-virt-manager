#include "_hosts.h"

_Hosts::_Hosts(QWidget *parent) :
    QWidget(parent)
{
    hosts = new QListWidget(this);
    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    name = new QLineEdit(this);
    name->setPlaceholderText("Host name");
    port = new QLineEdit(this);
    port->setPlaceholderText("Port");
    colon = new QLabel(":", this);
    panelLayout = new QHBoxLayout(this);
    panel = new QWidget(this);
    panelLayout->addWidget(add, 2);
    panelLayout->addWidget(name, 40);
    panelLayout->addWidget(colon, 1);
    panelLayout->addWidget(port, 8);
    panelLayout->addWidget(del, 2);
    panel->setLayout(panelLayout);
    baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(hosts);
    baseLayout->addWidget(panel);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    useHosts = new QCheckBox("Use Hosts", this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useHosts);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(useHosts, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(add, SIGNAL(clicked()),
            this, SLOT(addHost()));
    connect(del, SIGNAL(clicked()),
            this, SLOT(delHost()));
    setFullHostMode(true);
    setOneHostMode(false);
}

/* public slots */
bool _Hosts::isUsed() const
{
    return useHosts->isChecked();
}
void _Hosts::setUsage(bool state)
{
    useHosts->setChecked(state);
}
void _Hosts::setFullHostMode(bool state)
{
    hostMode = state;
}
QStringList _Hosts::getHostsList() const
{
    QStringList _list;
    for(int i = 0; i<hosts->count(); i++) {
        _list.append(hosts->item(i)->text());
    };
    return _list;
}
void _Hosts::setOneHostMode(bool state)
{
    oneHostMode = state;
}
void _Hosts::setHostItem(QString &_item)
{
    hosts->addItem(_item);
}
void _Hosts::clearHostList()
{
    hosts->clear();
}
void _Hosts::setHostPlaceholderText(const QString &s)
{
    name->setPlaceholderText(s);
}
void _Hosts::setPortPlaceholderText(const QString &s)
{
    port->setPlaceholderText(s);
}

/* private slots */
void _Hosts::addHost()
{
    if ( !name->text().isEmpty() &&
         ( !hostMode || !port->text().isEmpty() ) ) {
        QString _host = name->text();
        if ( !port->text().isEmpty() ) {
            _host.append(":");
            _host.append(port->text());
        };
        if ( hosts->findItems(_host, Qt::MatchExactly).isEmpty() ) {
            if ( oneHostMode ) hosts->clear();
            hosts->addItem(_host);
        };
        name->clear();
        port->clear();
        emit dataChanged();
    };
}
void _Hosts::delHost()
{
    QList<QListWidgetItem*> l = hosts->selectedItems();
    if ( !l.isEmpty() ) {
        hosts->takeItem(hosts->row(l.at(0)));
        emit dataChanged();
    };
    hosts->clearSelection();
}
