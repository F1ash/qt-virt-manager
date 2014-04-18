#ifndef _QWIDGET_H
#define _QWIDGET_H

#include <QWidget>
#include <QtXml/QDomDocument>
#include <QDebug>

class _QWidget : public QWidget
{
    Q_OBJECT
public:
    explicit _QWidget(QWidget *parent = 0);

public slots:
    virtual QDomNodeList getNodeList() const;

};

#endif // _QWIDGET_H
