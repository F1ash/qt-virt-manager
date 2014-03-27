#include "conn_item_index.h"

ConnItemIndex::ConnItemIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    URI = "";
    State = "";
    Data = DATA();
}
void ConnItemIndex::setName(QString &s)
{
    Name = s;
}
void ConnItemIndex::setURI(QString &s)
{
    URI = s;
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
QString ConnItemIndex::getURI() const
{
    return URI;
}
QString ConnItemIndex::getState() const
{
    return State;
}
DATA ConnItemIndex::getData() const
{
    return Data;
}
