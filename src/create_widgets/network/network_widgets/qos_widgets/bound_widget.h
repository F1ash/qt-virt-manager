#ifndef BOUND_WIDGET_H
#define BOUND_WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>

class Bound_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Bound_Widget(QWidget *parent = nullptr);
    QCheckBox      *averageL, *peakL,
                   *burstL, *floorL;
    QSpinBox       *average, *peak,
                   *burst, *floor;
    void            setFloorUsage(bool);

private:
    QGridLayout    *commonLayout;
};

#endif // BOUND_WIDGET_H
