#ifndef QSPICE_SMARTCARD_WIDGET_H
#define QSPICE_SMARTCARD_WIDGET_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include <QDialog>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSettings>
//#include <QDebug>

class QSpiceSmartcardWidget : public QDialog
{
    Q_OBJECT
public:
    explicit QSpiceSmartcardWidget(QWidget *parent = Q_NULLPTR);
    ~QSpiceSmartcardWidget();

signals:
    void             connectCard(QString&);
    void             disconnectCard(QString&);
    void             cardsChanged(void*);

private:
    QSettings        settings;
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

#endif // QSPICE_SMARTCARD_WIDGET_H
