#include "hugepages.h"

HugePages::HugePages(QWidget *parent) :
    QWidget(parent)
{
    pages = new QListWidget(this);
    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    size = new QSpinBox(this);
    size->setRange(0, 10485760);
    units = new QComboBox(this);
    units->addItems(QStringList()<<"KiB"<<"MiB"<<"GiB");
    nodeset = new QLineEdit(this);
    nodeset->setPlaceholderText("0-3,5");
    panelLayout = new QHBoxLayout(this);
    panel = new QWidget(this);
    panelLayout->addWidget(add, 2);
    panelLayout->addWidget(size, 10);
    panelLayout->addWidget(units, 2);
    panelLayout->addWidget(nodeset, 10);
    panelLayout->addWidget(del, 2);
    panel->setLayout(panelLayout);
    baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(pages);
    baseLayout->addWidget(panel);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(add, SIGNAL(clicked()),
            this, SLOT(addPage()));
    connect(del, SIGNAL(clicked()),
            this, SLOT(delPage()));
}

/* public slots */
QStringList HugePages::getPagesList() const
{
    QStringList _list;
    for(int i = 0; i<pages->count(); i++) {
        _list.append(pages->item(i)->text());
    };
    return _list;
}
void HugePages::setPageList(QString &page)
{
    pages->addItem(page);
}

/* private slots */
void HugePages::addPage()
{
    if ( !nodeset->text().isEmpty() && size->value()>0 ) {
        QString _page;
        _page.append(size->text());
        _page.append(":");
        _page.append(units->currentText());
        _page.append(":");
        _page.append(nodeset->text());
        pages->addItem(_page);
        size->setValue(0);
        units->setCurrentIndex(0);
        nodeset->clear();
        emit dataChanged();
    };
}
void HugePages::delPage()
{
    QList<QListWidgetItem*> l = pages->selectedItems();
    if ( !l.isEmpty() ) {
        pages->takeItem(pages->row(l.at(0)));
        emit dataChanged();
    };
    pages->clearSelection();
}
