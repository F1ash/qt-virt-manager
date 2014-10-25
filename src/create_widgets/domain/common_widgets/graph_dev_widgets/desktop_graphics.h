#ifndef DESKTOP_GRAPHICS_H
#define DESKTOP_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"

class Desktop_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit Desktop_Graphics(QWidget *parent = NULL);

private:
    QLabel          *displayLabel;
    QLineEdit       *display;
    QCheckBox       *fullscreen;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // DESKTOP_GRAPHICS_H
