#ifndef SPICESMARTCARDWIDGET_H
#define SPICESMARTCARDWIDGET_H

#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDebug>

class SpiceSmartcardWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SpiceSmartcardWidget(QWidget *parent = 0);

signals:
    void             connectCard(QString&);
    void             disconnectCard(QString&);
    void             cardsChanged(void*);

private:
    QListWidget     *smartcardList;
    QVBoxLayout     *commonLayout;

private slots:
    void             changeCardState(QListWidgetItem*);

public slots:
    void             addItem(QListWidgetItem*);
    void             clearList();
    void             addCard(QString&);
    void             removeCard(QString&);
};

#endif // SPICESMARTCARDWIDGET_H
