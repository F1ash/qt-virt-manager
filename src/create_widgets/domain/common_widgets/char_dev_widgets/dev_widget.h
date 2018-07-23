#ifndef DEV_WIDGET_H
#define DEV_WIDGET_H

#include "create_widgets/common/_qwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

class DevWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit DevWidget(
            QWidget *parent = Q_NULLPTR,
            QString  _tag   = "");

signals:

private:
    QLabel          *pathLabel;
    QLineEdit       *path;
    QGridLayout     *devLayout;
    QString          tag;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // DEV_WIDGET_H
