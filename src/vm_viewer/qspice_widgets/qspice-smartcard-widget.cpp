#include "qspice-smartcard-widget.h"

QSpiceSmartcardWidget::QSpiceSmartcardWidget(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Smartcards");
    smartcardList = new QListWidget(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(smartcardList);
    setLayout(commonLayout);
    setMinimumSize(100, 100);
    setContentsMargins(0,0,0,0);
    connect(smartcardList, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(changeCardState(QListWidgetItem*)));
}

/* private slots */
void QSpiceSmartcardWidget::changeCardState(QListWidgetItem *item)
{
    QString _id = item->text();
    //qDebug()<<item->text()<<item->checkState();
    if ( item->checkState()==Qt::Checked )
        emit connectCard(_id);
    else
        emit disconnectCard(_id);
}

/* public slots */
void QSpiceSmartcardWidget::addItem(QListWidgetItem *item)
{
    smartcardList->addItem(item);
}

void QSpiceSmartcardWidget::clearList()
{
    smartcardList->clear();
}

void QSpiceSmartcardWidget::addCard(QString &card)
{
    Q_UNUSED(card);
    emit cardsChanged((void*)this);
}

void QSpiceSmartcardWidget::removeCard(QString &card)
{
    Q_UNUSED(card);
    emit cardsChanged((void*)this);
}
