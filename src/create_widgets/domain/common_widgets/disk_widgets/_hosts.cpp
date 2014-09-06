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
}

/* public slots */
bool _Hosts::isUsed() const
{
    return useHosts->isChecked();
}
QStringList _Hosts::getHostsList() const
{
    QStringList _list;
    for(int i = 0; i<hosts->count(); i++) {
        _list.append(hosts->item(i)->text());
    };
    return _list;
}

/* private slots */
void _Hosts::addHost()
{
    if ( !name->text().isEmpty() && !port->text().isEmpty() ) {
        QString _host = QString("%1:%2")
                .arg(name->text()).arg(port->text());
        if ( hosts->findItems(_host, Qt::MatchExactly).isEmpty() )
            hosts->addItem(_host);
        name->clear();
        port->clear();
    };
}
void _Hosts::delHost()
{
    QList<QListWidgetItem*> l = hosts->selectedItems();
    if ( !l.isEmpty() ) {
        hosts->takeItem(hosts->row(l.at(0)));
    };
    hosts->clearSelection();
}
