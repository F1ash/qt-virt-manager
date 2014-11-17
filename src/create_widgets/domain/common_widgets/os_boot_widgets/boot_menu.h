#ifndef BOOT_MENU_H
#define BOOT_MENU_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>

class BootMenu : public QWidget
{
    Q_OBJECT
public:
    explicit BootMenu(QWidget *parent = 0);
    QCheckBox       *menu;
    QSpinBox        *timeOut;

signals:
    void             dataChanged();

private:
    QLabel          *timeLabel;
    QHBoxLayout     *commonLayout;
};

#endif // BOOT_MENU_H
