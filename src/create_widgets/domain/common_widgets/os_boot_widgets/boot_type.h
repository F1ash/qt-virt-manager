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
    explicit BootType(QWidget *parent = Q_NULLPTR);
    QComboBox       *bootType;
    QLineEdit       *osType;

private:
    QLabel          *typeLabel;
    QHBoxLayout     *commonLayout;
};

#endif // BOOT_TYPE_H
