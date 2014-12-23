#ifndef _IPV6_H
#define _IPV6_H

#include "create_widgets/domain/_qwidget.h"

class _IPv6 : public _QWidget
{
    Q_OBJECT
public:
    explicit _IPv6(QWidget *parent = NULL);

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // _IPV6_H
