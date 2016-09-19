#ifndef _ATTRIBUTES_H
#define _ATTRIBUTES_H

#include <QStackedWidget>
#include "create_widgets/common/_qwidget.h"
#include "data_types_widget/boolean_data.h"
#include "data_types_widget/ipsetflags_data.h"
#include "data_types_widget/ipv6_addr_data.h"
#include "data_types_widget/ipv6_mask_data.h"
#include "data_types_widget/ip_addr_data.h"
#include "data_types_widget/ip_mask_data.h"
#include "data_types_widget/mac_addr_data.h"
#include "data_types_widget/mac_mask_data.h"
#include "data_types_widget/string_data.h"
#include "data_types_widget/uint8_data.h"
#include "data_types_widget/uint16_data.h"
#include "data_types_widget/uint32_data.h"

class _Attributes : public _QWidget
{
public:
    explicit _Attributes(
            QWidget *parent = nullptr,
            QString  tag    = "Attribute:");
    virtual ~_Attributes()  = 0;
    QComboBox          *attrName;
    QStackedWidget     *attrEditor;
    virtual void        clearAllAttributeData();
    virtual void        setAttrValue(const QVariantMap&);
    virtual QVariantMap getAttrValue(QString&) const;
    QStringList         getAttrList() const;

private:
    QString             tag;
    QLabel             *attrLabel;
    QHBoxLayout        *attrLayout;
    QWidget            *attrWdg;
    QVBoxLayout        *commonLayout;
};

#endif // _ATTRIBUTES_H
