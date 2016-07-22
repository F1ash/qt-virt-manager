#ifndef _CHECKED_WIDGET_H
#define _CHECKED_WIDGET_H

#include "_qwidget.h"

class _Checked_Widget : public _QWidget
{
    Q_OBJECT
public:
    _Checked_Widget(
            QWidget *parent = nullptr,
            QString  _tag   = QString());
    const QString        tag;
    QVBoxLayout         *baseLayout;
    bool                 isUsed() const;
    bool                 isFreezed() const;
    void                 setUsageToolTip(const QString&);

signals:
    void                 toggled(bool);

private:
    QCheckBox           *usage;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

public slots:
    void                 setUsage(bool);
    void                 setCheckState( Qt::CheckState );
    void                 setFreez(bool);
    virtual QDomDocument getDataDocument() const;
};

#endif // _CHECKED_WIDGET_H
