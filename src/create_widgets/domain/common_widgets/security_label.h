#ifndef SECURITY_LABEL_H
#define SECURITY_LABEL_H

#include "create_widgets/domain/restore_panel.h"
#include "sec_label_widgets/sec_labels.h"

class SecurityLabel : public _QWidget
{
    Q_OBJECT
public:
    explicit SecurityLabel(
            QWidget *parent = NULL,
            QString  _xmlDesc = QString());

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    RestorePanel    *restorePanel;
    SecLabels       *secLabels;
    QVBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
    QString          closeDataEdit();

private slots:
    void             stateChanged();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetSecData();
    void             revertSecData();
    void             saveSecData();
};

#endif // SECURITY_LABEL_H
