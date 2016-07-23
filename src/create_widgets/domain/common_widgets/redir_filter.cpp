#include "redir_filter.h"

RedirFilter::RedirFilter(QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    _class = new QLineEdit(this);
    _class->setPlaceholderText("Class");
    _vendor = new QLineEdit(this);
    _vendor->setPlaceholderText("Vendor");
    _product = new QLineEdit(this);
    _product->setPlaceholderText("Product");
    _version = new QLineEdit(this);
    _version->setPlaceholderText("Version");

    panelLayout->insertWidget(1, _class, 10);
    panelLayout->insertWidget(2, _vendor, 10);
    panelLayout->insertWidget(3, _product, 10);
    panelLayout->insertWidget(4, _version, 10);
}
void RedirFilter::clearList()
{
    list->clear();
}

/* public slots */
QStringList RedirFilter::getFiltersList() const
{
    QStringList _list;
    for(int i = 0; i<list->count(); i++) {
        QListWidgetItem *_filterItem = list->item(i);
        QString _filter = _filterItem->text();
        _filter.append(":");
        Qt::CheckState _state = _filterItem->checkState();
        _filter.append( (_state==Qt::Checked)? "yes" : "no" );
        _list.append(_filter);
    };
    return _list;
}
void RedirFilter::setFiltersList(QString &_filter, bool _allow)
{
    QListWidgetItem *_item = new QListWidgetItem();
    _item->setText(_filter);
    _item->setCheckState( (_allow)? Qt::Checked : Qt::Unchecked );
    list->insertItem(0, _item);
}

/* private slots */
void RedirFilter::addItem()
{
    QString _c, _v, _p, _ver, _filter;
    _c = (_class->text().isEmpty())? "-1" : _class->text();
    _v = (_vendor->text().isEmpty())? "-1" : _vendor->text();;
    _p = (_product->text().isEmpty())? "-1" : _product->text();;
    _ver = (_version->text().isEmpty())? "-1" : _version->text();;
    // list format : <class:vendor:product:version>
    _filter = QString("%1:%2:%3:%4").arg(_c).arg(_v).arg(_p).arg(_ver);
    if ( list->findItems(_filter, Qt::MatchExactly).isEmpty() ) {
        QListWidgetItem *_filterItem = new QListWidgetItem(list);
        _filterItem->setText(_filter);
        _filterItem->setCheckState(Qt::Unchecked);
        list->insertItem(list->count(), _filterItem);
        emit dataChanged();
    };
    _class->clear();
    _vendor->clear();
    _product->clear();
    _version->clear();
}
void RedirFilter::delItem()
{
    QList<QListWidgetItem*> l = list->selectedItems();
    if ( !l.isEmpty() ) {
        list->takeItem(list->row(l.at(0)));
        emit dataChanged();
    };
    list->clearSelection();
}
