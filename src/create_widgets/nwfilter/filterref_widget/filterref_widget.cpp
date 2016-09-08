#include "filterref_widget.h"

FilterrefWidget::FilterrefWidget(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    filters = new QComboBox(this);
    baseLayout->insertWidget(0, filters);
}
void FilterrefWidget::setFilters(const QStringList &l)
{
    filters->addItems(l);
}
void FilterrefWidget::addItem()
{
    QString _filterName = filters->currentText();
    list->addItem(_filterName);
}
