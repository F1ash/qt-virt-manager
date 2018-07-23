#ifndef SECURITY_LABEL_H
#define SECURITY_LABEL_H

#include "create_widgets/domain/_tab.h"
#include "sec_label_widgets/sec_labels.h"

class SecurityLabel : public _Tab
{
    Q_OBJECT
public:
    explicit SecurityLabel(
            QWidget *parent = Q_NULLPTR,
            QString  _xmlDesc = QString());

private:
    const QString    xmlDesc;
    QString          capabilities;
    SecLabels       *secLabels;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             readXMLDesciption();
    void             readXMLDesciption(const QString&);
};

#endif // SECURITY_LABEL_H
