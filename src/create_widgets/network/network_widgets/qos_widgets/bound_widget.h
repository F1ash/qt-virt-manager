#ifndef BOUND_WIDGET_H
#define BOUND_WIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QGridLayout>

class Bound_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Bound_Widget(QWidget *parent = Q_NULLPTR);
    QCheckBox      *averageL, *peakL,
                   *burstL, *floorL;
    QSpinBox       *average, *peak,
                   *burst, *floor;
    void            setFloorUsage(bool);

private:
    QVBoxLayout    *commonLayout;
    QGridLayout    *lt;
    QWidget        *ltWdg;
};

#endif // BOUND_WIDGET_H
