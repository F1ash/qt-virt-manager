#include "redir_filter.h"

RedirFilter::RedirFilter(QWidget *parent) :
    QWidget(parent)
{
    filtered = new QCheckBox("Use filter:", this);
    filter = new QListWidget(this);
    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    _class = new QLineEdit(this);
    _class->setPlaceholderText("Class");
    _vendor = new QLineEdit(this);
    _vendor->setPlaceholderText("Vendor");
    _product = new QLineEdit(this);
    _product->setPlaceholderText("Product");
    _version = new QLineEdit(this);
    _version->setPlaceholderText("Version");

    panelLayout = new QHBoxLayout(this);
    panelLayout->addWidget(add, 1);
    panelLayout->addWidget(_class, 10);
    panelLayout->addWidget(_vendor, 10);
    panelLayout->addWidget(_product, 10);
    panelLayout->addWidget(_version, 10);
    panelLayout->addWidget(del, 1);
    panel = new QWidget(this);
    panel->setLayout(panelLayout);
    baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(filter);
    baseLayout->addWidget(panel);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(filtered);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(filtered, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(add, SIGNAL(clicked()),
            this, SLOT(addFilter()));
    connect(del, SIGNAL(clicked()),
            this, SLOT(delFilter()));
}

/* public slots */
bool RedirFilter::isFiltered() const
{
    return filtered->isChecked();
}
QStringList RedirFilter::getFiltersList() const
{
    QStringList _list;
    for(int i = 0; i<filter->count(); i++) {
        QListWidgetItem *_filterItem = filter->item(i);
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
    filter->insertItem(0, _item);
}

/* private slots */
void RedirFilter::addFilter()
{
    QString _c, _v, _p, _ver, _filter;
    _c = (_class->text().isEmpty())? "-1" : _class->text();
    _v = (_vendor->text().isEmpty())? "-1" : _vendor->text();;
    _p = (_product->text().isEmpty())? "-1" : _product->text();;
    _ver = (_version->text().isEmpty())? "-1" : _version->text();;
    // filter format : <class:vendor:product:version>
    _filter = QString("%1:%2:%3:%4").arg(_c).arg(_v).arg(_p).arg(_ver);
    if ( filter->findItems(_filter, Qt::MatchExactly).isEmpty() ) {
        QListWidgetItem *_filterItem = new QListWidgetItem(filter);
        _filterItem->setText(_filter);
        _filterItem->setCheckState(Qt::Unchecked);
        filter->insertItem(filter->count(), _filterItem);
        emit dataChanged();
    };
    _class->clear();
    _vendor->clear();
    _product->clear();
    _version->clear();
}
void RedirFilter::delFilter()
{
    QList<QListWidgetItem*> l = filter->selectedItems();
    if ( !l.isEmpty() ) {
        filter->takeItem(filter->row(l.at(0)));
        emit dataChanged();
    };
    filter->clearSelection();
}
