#ifndef POWER_H
#define POWER_H

/*
 * WARNING: Only qemu driver support
 */

#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>

class Power : public QWidget
{
    Q_OBJECT
public:
    explicit Power(QWidget *parent = 0);
    QCheckBox       *suspend_to_disk,
                    *suspend_to_mem;

signals:
    void             dataChanged();

private:
    QHBoxLayout     *commonLayout;
};

#endif // POWER_H
