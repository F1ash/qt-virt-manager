#ifndef FORWARD_WIDGET_H
#define FORWARD_WIDGET_H

#include "_checked_widget.h"
#include "forward_mode_widgets/nat_mode_widget.h"
#include "forward_mode_widgets/route_mode_widget.h"
#include "forward_mode_widgets/bridge_mode_widget.h"
#include "forward_mode_widgets/private_mode_widget.h"
#include "forward_mode_widgets/vepa_mode_widget.h"
#include "forward_mode_widgets/passthrough_mode_widget.h"
#include "forward_mode_widgets/hostdev_mode_widget.h"
#include <QStackedWidget>

class Forward_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit Forward_Widget(
            QWidget *parent = nullptr,
            QString  tag = "Forwards");
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

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             modeChanged(const QString&);
};

#endif // FORWARD_WIDGET_H
