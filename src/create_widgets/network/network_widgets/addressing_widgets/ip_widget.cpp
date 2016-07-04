#include "ip_widget.h"

/*
 * IP_Widget is set of IP_Elements,
 * which is a IP or stata route addressing.
 */

IP_Widget::IP_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    addIP = new AddTab(this);
    ipSet = new QTabWidget(this);
    ipSet->setTabsClosable(true);
    ipSet->setCornerWidget(addIP);
    ipSet->setContextMenuPolicy(Qt::CustomContextMenu);
    baseLayout->addWidget(ipSet);
    connect(addIP->addTabAction, SIGNAL(triggered(bool)),
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
        _IP_Widget *wdg =
                static_cast<_IP_Widget*>(ipSet->widget(i));
        if ( nullptr==wdg ) continue;
        doc.appendChild(wdg->getDataDocument());
    };
    return doc;
}
void IP_Widget::setDataDescription(QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network = doc.documentElement();
    if ( !_network.isNull() ) {
        QDomNode n = _network.firstChild();
        if ( !n.isNull() ) {
            int i = 0;
            while ( !n.isNull() ) {
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                    if ( e.tagName()=="ip" || e.tagName()=="route" ) {
                        setUsage(true);
                        if ( i>0 ) addTab();
                        _IP_Widget *wdg =
                                static_cast<_IP_Widget*>(ipSet->widget(i));
                        if ( nullptr!=wdg ) {
                            QDomDocument _doc;
                            _doc.setContent(QString());
                            _doc.appendChild(e.cloneNode());
                            QString _xml = _doc.toString();
                            wdg->setDataDescription(_xml);
                            ++i;
                        };
                    };
                };
                n = n.nextSibling();
            };
        };
    };
}

/* private slots */
void IP_Widget::updateDHCPUsage(uint ver, uint idx, bool state)
{
    switch (ver) {
    case 4:
        IPv4HasDHCP = state;
        break;
    case 6:
        IPv6HasDHCP = state;
        break;
    default:
        break;
    };
    qDebug()<<"IPv4:"<<IPv4HasDHCP;
    qDebug()<<"IPv6:"<<IPv6HasDHCP;

    for (int i=0; i<ipSet->count(); i++) {
        if ( i==idx ) continue;
        _IP_Widget *wdg =
                static_cast<_IP_Widget*>(ipSet->widget(i));
        if ( nullptr==wdg ) continue;
        wdg->updateDHCPUsage(ver, state);
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
                    IPv4HasDHCP,
                    IPv6HasDHCP),
                "IP Element");
    _IP_Widget *wdg =
            static_cast<_IP_Widget*>(ipSet->widget(i));
    if ( nullptr!=wdg ) {
        wdg->setTabIdx(i);
        connect(wdg, SIGNAL(dhcpUsageChanged(uint, uint, bool)),
                this, SLOT(updateDHCPUsage(uint, uint, bool)));
    };
}
void IP_Widget::closeTab(int i)
{
    if ( ipSet->count()<2 ) return;
    _IP_Widget *wdg =
            static_cast<_IP_Widget*>(ipSet->widget(i));
    if ( nullptr!=wdg ) {
        wdg->tabToClose();
        ipSet->removeTab(i);
        wdg->deleteLater();
        for (int i=0; i<ipSet->count(); i++) {
            _IP_Widget *wdg =
                    static_cast<_IP_Widget*>(ipSet->widget(i));
            if ( nullptr==wdg ) continue;
            // set new tab idx;
            wdg->setTabIdx(i);
        };
    };
}
