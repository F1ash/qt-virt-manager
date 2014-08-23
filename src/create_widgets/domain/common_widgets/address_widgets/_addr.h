#ifndef _ADDR_H
#define _ADDR_H

#include <QWidget>
#include <QMap>
#include <QString>

typedef QMap<QString, QString> AttrList;

class _Addr : public QWidget
{
    Q_OBJECT
public:
    explicit _Addr(QWidget *parent = 0);

signals:

public slots:
    virtual AttrList getAttrList() const;

};

#endif // _ADDR_H
