/*
   Copyright (C) 2014 Lindsay Mathieson (lindsay.mathieson@gmail.com).

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QSPICEOBJECT_H
#define QSPICEOBJECT_H

#include <spice/enums.h>

#include <QObject>

class QSpiceObject : public QObject
{
    Q_OBJECT
public:
    explicit QSpiceObject(QObject *parent = 0);

    virtual bool objectDisposed();

signals:

public slots:

protected:
    friend class QSpiceUsbDeviceManager;
    void *gobject;

    QSpiceObject(void *_gobject);

    int getPropInt(QString name);
    void setProp(QString name, const int i);

    QString getPropString(QString name);
    void setProp(QString name, const QString& s);

#define Q_GPROP_INT(name, propname) \
    inline int get ## name() {return getPropInt(propname);}; \
    inline void set ## name(const QString& v) {setProp(propname, v);}

#define Q_GPROP_STR(name, propname) \
    inline QString get ## name() {return getPropString(propname);}; \
    inline void set ## name(const QString& v) {setProp(propname, v);}

};


#endif // QSPICEOBJECT_H
