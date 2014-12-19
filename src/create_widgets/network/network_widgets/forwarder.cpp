#include "forwarder.h"

Forwarder::Forwarder(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    frwds = new QLineEdit(this);
    frwds->setPlaceholderText("8.8.8.8");
    panelLayout->insertWidget(1, frwds);
}

/* public slots */
QDomDocument Forwarder::getDataDocument() const
{
    QDomDocument doc;
    return doc;
}
void Forwarder::addItem()
{

}
