#ifndef UNIX_WIDGET_H
#define UNIX_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>

class UnixWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit UnixWidget(QWidget *parent = 0);
    ~UnixWidget();

signals:

private:
    QLabel          *pathLabel;
    QLineEdit       *path;
    QLabel          *modeLabel;
    QComboBox       *mode;
    QGridLayout     *unixLayout;

public slots:
    QDomNodeList getNodeList() const;

};

#endif // UNIX_WIDGET_H
