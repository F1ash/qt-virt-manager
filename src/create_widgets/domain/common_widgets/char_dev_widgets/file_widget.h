#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class FileWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit FileWidget(QWidget *parent = 0, QString _tag = "");

signals:

private:
    QLabel          *pathLabel;
    QLineEdit       *path;
    QGridLayout     *fileLayout;
    QString          tag;

public slots:
    QDomDocument getDevDocument() const;

};

#endif // FILE_WIDGET_H
