#ifndef RANDOM_H
#define RANDOM_H

#include <QStackedWidget>
#include "char_device.h"

class Random : public _QWidget
{
    Q_OBJECT
public:
    explicit Random(QWidget *parent = Q_NULLPTR);
    QComboBox       *model, *bkModel;
    QCheckBox       *rate;
    QSpinBox        *period, *bytes;
    QComboBox       *bkRandom;
    CharDevice      *bkEGD;
    QStackedWidget  *bkWdg;

private:
    QLabel          *modelLabel, *periodLabel,
                    *bytesLabel, *bkModelLabel;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QLabel          *bkRandomLabel;
    QHBoxLayout     *bkRandomLayout;
    QWidget         *bkRandomWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             rateChecked(bool);
    void             modelChanged(int);
};

#endif // RANDOM_H
