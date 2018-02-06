#ifndef SRV_DNS_H
#define SRV_DNS_H

#include "create_widgets/common/_list_widget.h"

class SRV_DNS : public _List_Widget
{
    Q_OBJECT
public:
    explicit SRV_DNS(
            QWidget *parent = nullptr,
            QString  tag    = "SRVs");

private:
    QLabel          *serviceL, *domainL, *targetL,
                    *portL, *priorityL, *weightL;
    QLineEdit       *service, *domain, *target;
    QSpinBox        *port, *priority, *weight;
    QComboBox       *protocol;
    QHBoxLayout     *mandatoryLayout;
    QGridLayout     *lettersLayout, *numbersLayout;
    QWidget         *mandatory, *letters, *numbers;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             addItem();
};

#endif // SRV_DNS_H
