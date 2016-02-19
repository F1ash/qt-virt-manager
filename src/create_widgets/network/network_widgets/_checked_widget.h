#ifndef _CHECKED_WIDGET_H
#define _CHECKED_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class _Checked_Widget : public _QWidget
{
    Q_OBJECT
public:
    _Checked_Widget(
            QWidget *parent = nullptr,
            QString  _tag   = QString());
    const QString        tag;
    QVBoxLayout         *baseLayout;

signals:
    void                 toggled(bool);

private:
    QCheckBox           *usage;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

public slots:
    bool                 isUsed() const;
    void                 setUsage(bool);
    void                 setCheckState( Qt::CheckState );
    void                 setFreez(bool);
    virtual QDomDocument getDataDocument() const;
};

#endif // _CHECKED_WIDGET_H
