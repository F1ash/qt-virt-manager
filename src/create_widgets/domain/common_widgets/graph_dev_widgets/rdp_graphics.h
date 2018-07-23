#ifndef RDP_GRAPHICS_H
#define RDP_GRAPHICS_H

#include "create_widgets/common/_qwidget.h"

class RDP_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit RDP_Graphics(QWidget *parent = Q_NULLPTR);

private:
    QCheckBox       *autoPort;
    QSpinBox        *port;
    QCheckBox       *multiUser;
    QCheckBox       *replaceUser;
    QGridLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             autoPortChanged(bool);
};

#endif // RDP_GRAPHICS_H
