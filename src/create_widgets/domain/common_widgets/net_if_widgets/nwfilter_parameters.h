#ifndef NWFILTER_PARAMETERS_H
#define NWFILTER_PARAMETERS_H

#include "create_widgets/common/_list_widget.h"

class NWFilter_Params : public _List_Widget
{
    Q_OBJECT
public:
    explicit NWFilter_Params(
            QWidget *parent = Q_NULLPTR,
            QString  tag    = "");
    void             setNWFiltersList(const QStringList&);

private:
    QComboBox       *nwFilters;
    QCheckBox       *useAutoIP;
    QComboBox       *autoIpTypes;
    QHBoxLayout     *autoIpLayout;
    QWidget         *autoIP;
    QComboBox       *paramName;
    QLineEdit       *paramVal;

public slots:
    void             addItem();
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             variableChanged(int);
};

#endif // NWFILTER_PARAMETERS_H
