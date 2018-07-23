#ifndef POWER_H
#define POWER_H

/*
 * WARNING: Only qemu driver support
 */

#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include "create_widgets/common/_changed.h"

class Power : public _Changed
{
    Q_OBJECT
public:
    explicit Power(QWidget *parent = Q_NULLPTR);
    QCheckBox       *suspend_to_disk,
                    *suspend_to_mem;

signals:
    //void             dataChanged();

private:
    QHBoxLayout     *commonLayout;
};

#endif // POWER_H
