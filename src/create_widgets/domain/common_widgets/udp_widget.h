#ifndef UDP_WIDGET_H
#define UDP_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class UdpWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit UdpWidget(QWidget *parent = 0);
    ~UdpWidget();

signals:

private:

    QGridLayout     *udpLayout;

public slots:
    QDomNodeList getNodeList() const;

};

#endif // UDP_WIDGET_H
