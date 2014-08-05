#ifndef TCP_WIDGET_H
#define TCP_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class TcpWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit TcpWidget(QWidget *parent = 0);
    ~TcpWidget();

signals:

private:

    QGridLayout     *tcpLayout;

public slots:
    QDomNodeList getNodeList() const;

};

#endif // TCP_WIDGET_H
