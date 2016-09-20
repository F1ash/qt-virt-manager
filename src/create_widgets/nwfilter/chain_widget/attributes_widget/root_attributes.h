#ifndef ROOT_ATTRIBUTES_H
#define ROOT_ATTRIBUTES_H

#include "_attributes.h"

class ROOT_Attributes : public _Attributes
{
    Q_OBJECT
public:
    explicit ROOT_Attributes(
            QWidget *parent = nullptr,
            QString  tag = "Protocol:");
    void        clearAllAttributeData();
    void        setAttrValue(const QVariantMap&);
    QVariantMap getAttrValue(QString&) const;
    QStringList getAttrList() const;

private slots:
    void        dataEdited();
};

#endif // ROOT_ATTRIBUTES_H
