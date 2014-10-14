#ifndef UNIX_WIDGET_H
#define UNIX_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class UnixWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit UnixWidget(QWidget *parent = NULL, QString _tag = "");

signals:

private:
    QLabel          *pathLabel;
    QLineEdit       *path;
    QLabel          *modeLabel;
    QComboBox       *mode;
    QGridLayout     *unixLayout;
    QString          tag;

public slots:
    QDomDocument     getDataDocument() const;
    void             setPath(QString);
    void             setDataDescription(QString&);
};

#endif // UNIX_WIDGET_H
