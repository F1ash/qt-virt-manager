#ifndef FORWARD_WIDGET_H
#define FORWARD_WIDGET_H

#include "create_widgets/common/_checked_widget.h"
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
            QWidget *parent = Q_NULLPTR,
            QString  tag    = "Forward");
    QString          getCurrentMode() const;
    void             setCurrentMode(const QString&);

signals:
    void             optionalsNeed(bool);
    void             QoSAvailable(bool);

private:
    QComboBox       *mode;
    QCheckBox       *devLabel;
    QLineEdit       *dev;
    QLabel          *modeLabel;
    QHBoxLayout     *devLayout, *frwdLayout;
    QWidget         *devWdg;
    QWidget         *forwards;
    QStackedWidget  *frwdModeSet;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             modeChanged(const QString&);
};

#endif // FORWARD_WIDGET_H
