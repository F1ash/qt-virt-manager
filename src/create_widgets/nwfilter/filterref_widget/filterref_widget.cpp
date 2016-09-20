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
void FilterrefWidget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _filter;
    _filter = doc.firstChildElement("filter");
    if ( !_filter.isNull() ) {
        QDomNode _n = _filter.firstChild();
        while ( !_n.isNull() ) {
            setUsage(true);
            QDomElement _el = _n.toElement();
            if ( !_el.isNull() ) {
                if ( _el.tagName()=="filterref" ) {
                    QString _filterName =
                            _el.attribute("filter");
                    list->addItem(_filterName);
                };
            };
            _n = _n.nextSibling();
        };
    };
}
QDomDocument FilterrefWidget::getDataDocument() const
{
    QDomDocument doc;
    for (uint i=0; i<list->count(); i++) {
        QListWidgetItem *item = list->item(i);
        if ( item!=nullptr ) {
            QString _filterName = item->text();
            QDomElement _filterref = doc.createElement("filterref");
            _filterref.setAttribute("filter", _filterName);
            doc.appendChild(_filterref);
        };
    };
    return doc;
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
