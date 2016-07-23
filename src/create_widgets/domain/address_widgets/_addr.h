#ifndef _ADDR_H
#define _ADDR_H

#include <QMap>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <create_widgets/common/_changed.h>

typedef QMap<QString, QString> AttrList;

class _Addr : public _Changed
{
    Q_OBJECT
public:
    explicit _Addr(QWidget *parent = nullptr);

public slots:
    virtual AttrList getAttrList() const;
};

#endif // _ADDR_H
