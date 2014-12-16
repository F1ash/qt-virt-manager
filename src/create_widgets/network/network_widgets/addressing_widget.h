#ifndef ADDRESSING_WIDGET_H
#define ADDRESSING_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include <QLineEdit>
#include <QSpinBox>

class Addressing_Widget : public _QWidget
{
    Q_OBJECT
public:
    explicit Addressing_Widget(QWidget *parent = NULL);

private:
    QCheckBox       *usage;
    QVBoxLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
};

#endif // ADDRESSING_WIDGET_H
