#ifndef FILTERREF_WIDGET_H
#define FILTERREF_WIDGET_H

#include "create_widgets/common/_list_widget.h"

class FilterrefWidget : public _List_Widget
{
    Q_OBJECT
public:
    explicit FilterrefWidget(
            QWidget *parent = nullptr,
            QString  tag    = "");
    void             setFilters(const QStringList&);

signals:

private:
    QComboBox       *filters;

public slots:
    void             addItem();
};

#endif // FILTERREF_WIDGET_H
