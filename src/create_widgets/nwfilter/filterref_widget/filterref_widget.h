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
    void             setDataDescription(const QString&);
    QDomDocument     getDataDocument() const;

signals:

private:
    QComboBox       *filters;
    QPushButton     *up, *down;

public slots:
    void             addItem();
    void             itemUp();
    void             itemDown();
};

#endif // FILTERREF_WIDGET_H
