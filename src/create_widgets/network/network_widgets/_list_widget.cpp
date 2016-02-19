#include "_list_widget.h"

_List_Widget::_List_Widget(QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    list = new QListWidget(this);
    add = new QPushButton(QIcon::fromTheme("list-add"), "", this);
    del = new QPushButton(QIcon::fromTheme("list-remove"), "", this);
    panelLayout = new QHBoxLayout();
    panelLayout->addWidget(add);
    panelLayout->addWidget(del);
    panel = new QWidget(this);
    panel->setLayout(panelLayout);
    baseLayout->addWidget(list);
    baseLayout->addWidget(panel);
    connect(add, SIGNAL(clicked()),
            this, SLOT(addItem()));
    connect(del, SIGNAL(clicked()),
            this, SLOT(delItem()));
}

/* public slots */
void _List_Widget::addItem()
{
}
void _List_Widget::delItem()
{
    int row = list->currentRow();
    QListWidgetItem *_item = list->takeItem(row);
    if ( nullptr!=_item ) {
        delete _item;
        _item = nullptr;
    };
}
