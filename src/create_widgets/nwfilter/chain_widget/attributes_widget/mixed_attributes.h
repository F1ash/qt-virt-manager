#ifndef MIXED_ATTRIBUTES_H
#define MIXED_ATTRIBUTES_H

#include "_attributes.h"

class MIXED_Attributes : public _Attributes
{
    Q_OBJECT
public:
    explicit MIXED_Attributes(
            QWidget *parent = nullptr,
            QString  tag = "Protocol:");
    void        clearAllAttributeData();

signals:
    void        released(bool);

private slots:
    void        dataEdited();
};

#endif // MIXED_ATTRIBUTES_H
