#ifndef RESIZE_DIALOG_H
#define RESIZE_DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class ResizeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ResizeDialog(
            QWidget *parent = NULL,
            unsigned long long i = 0);
    ~ResizeDialog();

private:
    const unsigned long long   bytes = 1;
    const unsigned long long   KiB   = 1024;
    const unsigned long long   MiB   = 1048576;
    const unsigned long long   GiB   = 1073741824;
    const unsigned long long   TiB   = 1099511627776;
    unsigned long long   _size, currentRange;
    QLabel              *sizeLabel;
    QSpinBox            *size;

    QPushButton         *ok;
    QPushButton         *cancel;

    QHBoxLayout         *buttonLayout;
    QWidget             *buttons;
    QVBoxLayout         *commonlayout;

private slots:
    void okClicked();
    void cancelClicked();
    void changeRange(int);
    void changeRangeLong(unsigned long long);

public slots:
    unsigned long long getNewSize() const;

};

#endif // RESIZE_DIALOG_H
