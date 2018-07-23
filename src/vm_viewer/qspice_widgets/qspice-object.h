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

#ifndef QSPICE_OBJECT_H
#define QSPICE_OBJECT_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include <QObject>
#include <QStringList>
//#include <QDebug>

class QSpiceObject : public QObject
{
    Q_OBJECT
public:
    explicit QSpiceObject(QObject *parent = Q_NULLPTR);
    virtual ~QSpiceObject()               = 0;
    virtual bool objectDisposed();

protected:
    void *gobject;
    QSpiceObject(void *_gobject);

    int getPropInt(QString name);
    void setProp(QString name, const int i);

    QString getPropString(QString name);
    void setProp(QString name, const QString &s);

    bool getPropBool(QString name);
    void setProp(QString name, const bool v);

    void* getPropPointer(QString name);
    void  setProp(QString name, const void *v);

    QStringList getPropStrList(QString name);
    void  setProp(QString name, const QStringList &v);

#define Q_GPROP_INT(name, propname) \
    inline int get ## name() {return getPropInt(propname);}; \
    inline void set ## name(const int v) {setProp(propname, v);}

#define Q_GPROP_STR(name, propname) \
    inline QString get ## name() {return getPropString(propname);}; \
    inline void set ## name(const QString &v) {setProp(propname, v);}

#define Q_GPROP_BOOL(name, propname) \
    inline bool get ## name() {return getPropBool(propname);}; \
    inline void set ## name(const bool v) {setProp(propname, v);}

#define Q_GPROP_POINTER(name, propname) \
    inline void* get ## name() {return getPropPointer(propname);}; \
    inline void  set ## name(const void* v) {setProp(propname, v);}

#define Q_GPROP_STRLIST(name, propname) \
    inline QStringList get ## name() {return getPropStrList(propname);}; \
    inline void  set ## name(const QStringList &v) {setProp(propname, v);}
};


#endif // QSPICE_OBJECT_H
