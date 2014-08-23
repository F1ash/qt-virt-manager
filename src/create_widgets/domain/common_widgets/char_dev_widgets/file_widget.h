#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class FileWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit FileWidget(QWidget *parent = 0);
    ~FileWidget();

signals:

private:
    QLabel          *pathLabel;
    QLineEdit       *path;
    QGridLayout     *fileLayout;

public slots:
    QDomNodeList getNodeList() const;

};

#endif // FILE_WIDGET_H
