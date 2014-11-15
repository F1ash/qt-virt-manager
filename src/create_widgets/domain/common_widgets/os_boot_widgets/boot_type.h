#ifndef BOOT_TYPE_H
#define BOOT_TYPE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>

class BootType : public QWidget
{
    Q_OBJECT
public:
    explicit BootType(QWidget *parent = 0);
    QComboBox       *bootType;
    QLineEdit       *osType;

private:
    QLabel          *typeLabel;
    QHBoxLayout     *commonLayout;

public slots:

};

#endif // BOOT_TYPE_H
