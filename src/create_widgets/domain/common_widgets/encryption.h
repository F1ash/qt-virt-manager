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
    explicit Encryption(QWidget *parent = nullptr);
    bool                 isUsed() const;
    void                 setUsage(const bool);
    QString              getFormat() const;
    void                 setFormat(const QString&);

private:
    QCheckBox           *useEncryption;
    QLabel              *formatLabel;
    QComboBox           *format;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;
};

#endif // ENCRYPTION_H
