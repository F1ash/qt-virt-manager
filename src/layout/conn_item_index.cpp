#include "conn_item_index.h"

ConnItemIndex::ConnItemIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    Host = "";
    State = "";
    Data = DATA();
}
void ConnItemIndex::setName(QString &s)
{
    Name = s;
}
void ConnItemIndex::setHost(QString &s)
{
    Host = s;
}
void ConnItemIndex::setState(QString &s)
{
    State = s;
}
void ConnItemIndex::setData(DATA &_data)
{
    Data = _data;
}
QString ConnItemIndex::getName() const
{
    return Name;
}
QString ConnItemIndex::getHost() const
{
    return Host;
}
QString ConnItemIndex::getState() const
{
    return State;
}
DATA ConnItemIndex::getData() const
{
    return Data;
}
