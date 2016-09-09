#ifndef MIXED_ATTRIBUTES_H
#define MIXED_ATTRIBUTES_H

#include "_attributes.h"

class MIXED_Attributes : public _Attributes
{
public:
    explicit MIXED_Attributes(
            QWidget *parent = nullptr,
            QString  tag = "Protocol:");
};

#endif // MIXED_ATTRIBUTES_H
