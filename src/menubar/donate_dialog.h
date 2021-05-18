#ifndef DONATE_DIALOG_H
#define DONATE_DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include "create_widgets/common/click_label.h"

class Donate_Dialog : public QDialog
{
    Q_OBJECT
public:
    Donate_Dialog(QWidget *parent = Q_NULLPTR);
    //bool             showAgain() const;

private:
    Click_Label     //*donate1,
                    *donate2;
                    //*donate3, *donate4;
    QGridLayout     *donateLayout;
    QWidget         *donateWdg;
    //QCheckBox       *again;
    QVBoxLayout     *commonLayout;

private slots:
    void             donationClicked();
};

#endif // DONATE_DIALOG_H
