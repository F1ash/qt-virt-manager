#ifndef MEMORY_H
#define MEMORY_H

#include <QScrollArea>
#include "create_widgets/domain/_qwidget.h"
#include "memory_widgets/hugepages.h"

class Memory : public _QWidget
{
    Q_OBJECT
public:
    explicit Memory(
            QWidget *parent = 0,
            QString  arg1 = "",
            QString  arg2 = "",
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
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
    HugePages       *hugePagesList;
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

    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDevDocument() const;

private slots:
    void             changeCurrentMemValue(int);
    void             changeMaximumMemValue(int);
    void             readXMLDesciption();
    quint64          convertNiBtoKiB(quint64, QString&);
};

#endif // MEMORY_H
