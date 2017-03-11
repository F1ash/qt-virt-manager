#include "_hosts.h"

_Hosts::_Hosts(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    name = new QLineEdit(this);
    name->setPlaceholderText("Host name");
    port = new QLineEdit(this);
    port->setPlaceholderText("Port");
    panelLayout->insertWidget(1, name, 32);
    panelLayout->insertWidget(2, port, 8);
    setFullHostMode(true);
    setOneHostMode(false);
}

/* public slots */
void _Hosts::setFullHostMode(bool state)
{
    hostMode = state;
}
QStringList _Hosts::getHostsList() const
{
    QStringList _list;
    for(int i = 0; i<list->count(); i++) {
        _list.append(list->item(i)->text());
    };
    return _list;
}
void _Hosts::setOneHostMode(bool state)
{
    oneHostMode = state;
}
void _Hosts::setHostItem(const QString &_item)
{
    list->addItem(_item);
}
void _Hosts::clearHostList()
{
    list->clear();
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
void _Hosts::addItem()
{
    if ( !name->text().isEmpty() &&
         ( !hostMode || !port->text().isEmpty() ) ) {
        QString _host = name->text();
        if ( !port->text().isEmpty() ) {
            _host.append(":");
            _host.append(port->text());
        };
        if ( list->findItems(_host, Qt::MatchExactly).isEmpty() ) {
            if ( oneHostMode ) list->clear();
            list->addItem(_host);
        };
        name->clear();
        port->clear();
        emit dataChanged();
    };
}
void _Hosts::delItem()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( !l.isEmpty() ) {
        list->takeItem(list->row(l.at(0)));
        emit dataChanged();
    };
    list->clearSelection();
}
