#ifndef MEMORY_H
#define MEMORY_H

#include "create_widgets/domain/_qwidget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>

class Memory : public _QWidget
{
    Q_OBJECT
public:
    explicit Memory(QWidget *parent = 0, QString arg1 = "", QString arg2 = "");
    ~Memory();

signals:

private:
    QString          memUnit;
    QString          memValue;
    QLabel          *hostMemory;
    QLabel          *maxMemLabel;
    QLabel          *currMemLabel;
    QSpinBox        *maxMemValue;
    QSpinBox        *currMemValue;
    QGridLayout     *memLayout;
    QWidget         *memWdg;

    QCheckBox       *enableMemBacking;
    QCheckBox       *hugepages;
    QCheckBox       *nosharepages;
    QCheckBox       *locked;
    QGridLayout     *memBackingLayout;
    QWidget         *memBackingWdg;

    QCheckBox       *enableMemTune;
    QLabel          *hardLabel;
    QSpinBox        *hard_limit;
    QLabel          *softLabel;
    QSpinBox        *soft_limit;
    QLabel          *swapLabel;
    QSpinBox        *swap_hard_limit;
    QLabel          *guaranteeLabel;
    QSpinBox        *min_guarantee;
    QGridLayout     *memTuneLayout;
    QWidget         *memTuneWdg;

    QVBoxLayout     *commonLayout;

public slots:
    QDomNodeList getNodeList() const;

private slots:
    void changeCurrentMemValue(int);
    void changeMaximumMemValue(int);

};

#endif // MEMORY_H
