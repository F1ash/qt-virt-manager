#ifndef BOOT_MENU_H
#define BOOT_MENU_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include "create_widgets/common/_changed.h"

class BootMenu : public _Changed
{
    Q_OBJECT
public:
    explicit BootMenu(QWidget *parent = Q_NULLPTR);
    QCheckBox       *menu;
    QSpinBox        *timeOut;

signals:
    //void             dataChanged();

private:
    QLabel          *timeLabel;
    QHBoxLayout     *commonLayout;
};

#endif // BOOT_MENU_H
