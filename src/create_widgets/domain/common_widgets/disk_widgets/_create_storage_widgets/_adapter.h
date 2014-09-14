#ifndef _ADAPTER_H
#define _ADAPTER_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>

class _Adapter : public QWidget
{
    Q_OBJECT
public:
    explicit _Adapter(QWidget *parent = 0);
    QLineEdit           *name, *wwnn, *wwpn;
    QComboBox           *type;

private:
    QLabel              *nameLabel, *typeLabel,
                        *wwnnLabel, *wwpnLabel;
    QGridLayout         *commonLayout;

public slots:

private slots:
    void typeAdapterChanged(QString);
};

#endif // _ADAPTER_H
