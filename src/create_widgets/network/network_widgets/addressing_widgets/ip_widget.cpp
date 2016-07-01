#include "ip_widget.h"

IP_Widget::IP_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    addIPSet = new QPushButton(this);
    addIPSet->setText("Add IP Element");
    addIPSet->setIcon(QIcon::fromTheme("list-add"));
    addIPSet->setIconSize(QSize(
                this->fontInfo().pixelSize(),
                this->fontInfo().pixelSize()));
    ipSet = new QTabWidget(this);
    ipSet->setTabsClosable(true);
    ipSet->setContextMenuPolicy(Qt::CustomContextMenu);
    baseLayout->addWidget(addIPSet);
    baseLayout->addWidget(ipSet);
    connect(addIPSet, SIGNAL(released()),
            this, SLOT(addTab()));
    connect(ipSet, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
    connect(ipSet, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showCustomMenu(QPoint)));
    addTab();
}

/* public slots */
QDomDocument IP_Widget::getDataDocument() const
{
    QDomDocument doc;
    doc.setContent(QString());
    for (int i=0; i<ipSet->count(); i++) {
        _IP_Widget *wdg = static_cast<_IP_Widget*>(ipSet->widget(i));
        if ( nullptr==wdg ) continue;
        doc.appendChild(wdg->getDataDocument());
    };
    return doc;
}

/* private slots */
void IP_Widget::updateDHCPUsage()
{
    //qDebug()<<"IPv4:"<<IPv4HasDHCP;
    //qDebug()<<"IPv6:"<<IPv6HasDHCP;

    for (int i=0; i<ipSet->count(); i++) {
        _IP_Widget *wdg = static_cast<_IP_Widget*>(ipSet->widget(i));
        if ( nullptr==wdg ) continue;
        wdg->updateDHCPUsage();
    };
}
void IP_Widget::showCustomMenu(QPoint pos)
{
    AddTab *addMenu = new AddTab(this);
    connect(addMenu->addTabAction, SIGNAL(triggered()),
            this, SLOT(addTab()));
    addMenu->move(ipSet->mapToGlobal(pos));
    addMenu->exec();
    disconnect(addMenu->addTabAction, SIGNAL(triggered()),
               this, SLOT(addTab()));
    addMenu->deleteLater();
}
void IP_Widget::addTab()
{
    int i = ipSet->addTab(
                new _IP_Widget(
                    this,
                    &IPv4HasDHCP,
                    &IPv6HasDHCP),
                "IP Element");
    _IP_Widget *wdg = static_cast<_IP_Widget*>(ipSet->widget(i));
    if ( nullptr!=wdg ) {
        connect(wdg, SIGNAL(dhcpUsageChanged()),
                this, SLOT(updateDHCPUsage()));
    };
}
void IP_Widget::closeTab(int i)
{
    if ( ipSet->count()<2 ) return;
    _IP_Widget *wdg = static_cast<_IP_Widget*>(ipSet->widget(i));
    if ( nullptr!=wdg ) {
        wdg->ipv6->click();
        wdg->updateDHCPUsage();
        disconnect(wdg, SIGNAL(dhcpUsageChanged()),
                   this, SLOT(updateDHCPUsage()));
        ipSet->removeTab(i);
        wdg->deleteLater();
    };
}
