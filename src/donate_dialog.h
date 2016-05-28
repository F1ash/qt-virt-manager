#ifndef DONATE_DIALOG_H
#define DONATE_DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QHBoxLayout>
#include "vm_viewer/click_label.h"

class Donate_Dialog : public QDialog
{
    Q_OBJECT
public:
    Donate_Dialog(QWidget *parent = nullptr);
    bool showAgain() const;

private:
    Click_Label     *donate1;
    QCheckBox       *again;
    QVBoxLayout     *commonLayout;

private slots:
    void donation1Clicked();
};

#endif // DONATE_DIALOG_H
