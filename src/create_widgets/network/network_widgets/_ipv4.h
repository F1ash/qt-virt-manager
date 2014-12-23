#ifndef _IPV4_H
#define _IPV4_H

#include "create_widgets/domain/_qwidget.h"

class _IPv4 : public _QWidget
{
    Q_OBJECT
public:
    explicit _IPv4(QWidget *parent = NULL);

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // _IPV4_H
