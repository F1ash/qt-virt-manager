#ifndef FORWARD_WIDGET_H
#define FORWARD_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include "nat_mode_widget.h"
#include "route_mode_widget.h"
#include "bridge_mode_widget.h"
#include "private_mode_widget.h"
#include "vepa_mode_widget.h"
#include "passthrough_mode_widget.h"
#include "hostdev_mode_widget.h"
#include <QStackedWidget>

class Forward_Widget : public _QWidget
{
    Q_OBJECT
public:
    explicit Forward_Widget(QWidget *parent = 0);
    QCheckBox       *title;
    QComboBox       *mode;
    QCheckBox       *devLabel;
    QLineEdit       *dev;

signals:
    void             optionalsNeed(bool);

private:
    QLabel          *modeLabel;
    QHBoxLayout     *devLayout;
    QWidget         *devWdg;
    QGridLayout     *frwdLayout;
    QWidget         *forwards;
    QStackedWidget  *frwdModeSet;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             modeChanged(const QString&);
};

#endif // FORWARD_WIDGET_H
