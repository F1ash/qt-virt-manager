#include "spicesmartcardwidget.h"

SpiceSmartcardWidget::SpiceSmartcardWidget(QWidget *parent) :
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
void SpiceSmartcardWidget::changeCardState(QListWidgetItem *item)
{
    QString _id = item->text();
    //qDebug()<<item->text()<<item->checkState();
    if ( item->checkState()==Qt::Checked )
        emit connectCard(_id);
    else
        emit disconnectCard(_id);
}

/* public slots */
void SpiceSmartcardWidget::addItem(QListWidgetItem *item)
{
    smartcardList->addItem(item);
}

void SpiceSmartcardWidget::clearList()
{
    smartcardList->clear();
}

void SpiceSmartcardWidget::addCard(QString &dev)
{
    Q_UNUSED(dev);
    emit cardsChanged((void*)this);
}

void SpiceSmartcardWidget::removeCard(QString &dev)
{
    Q_UNUSED(dev);
    emit cardsChanged((void*)this);
}
