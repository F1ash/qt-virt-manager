#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

class Encryption : public QWidget
{
    Q_OBJECT
public:
    explicit Encryption(QWidget *parent = NULL);

private:
    QCheckBox           *useEncryption;
    QLabel              *formatLabel;
    QComboBox           *format;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

public slots:

};

#endif // ENCRYPTION_H
