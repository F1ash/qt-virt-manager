#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class FileWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit FileWidget(QWidget *parent = nullptr, QString _tag = "");

private:
    QLabel          *pathLabel;
    QLineEdit       *path;
    QGridLayout     *fileLayout;
    QString          tag;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // FILE_WIDGET_H
