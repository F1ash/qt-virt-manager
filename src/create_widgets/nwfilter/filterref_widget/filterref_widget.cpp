#include "filterref_widget.h"

FilterrefWidget::FilterrefWidget(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    filters = new QComboBox(this);
    baseLayout->insertWidget(0, filters);
    up = new QPushButton(
                QIcon::fromTheme("go-up"),
                "",
                this);
    down = new QPushButton(
                QIcon::fromTheme("go-down"),
                "",
                this);
    panelLayout->insertWidget(1, up);
    panelLayout->insertWidget(2, down);
    connect(up, SIGNAL(clicked(bool)),
            this, SLOT(itemUp()));
    connect(down, SIGNAL(clicked(bool)),
            this, SLOT(itemDown()));
}
void FilterrefWidget::readXmlDescData(const QString &_xmlDesc)
{

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
void FilterrefWidget::itemUp()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = list->row(item);
    list->takeItem(row);
    list->insertItem(row-1, item);
    list->setCurrentRow(row-1);
}
void FilterrefWidget::itemDown()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( l.isEmpty() || l.at(0)==nullptr ) return;
    QListWidgetItem *item = l.at(0);
    int row = list->row(item);
    list->takeItem(row);
    list->insertItem(row+1, item);
    list->setCurrentRow(row+1);
}
