#ifndef QSPICE_SMARTCARDMANAGER_H
#define QSPICE_SMARTCARDMANAGER_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-object.h"

class QSpiceSmartcardManager : public QSpiceObject
{
    Q_OBJECT
public:
    explicit QSpiceSmartcardManager(QObject *parent = Q_NULLPTR);

signals:
    void        cardInserted(QString&);
    void        cardRemoved(QString&);
    void        readerAdded(QString&);
    void        readerRemoved(QString&);

protected:
    friend class QSpiceHelper;
    void        init();

public slots:
    QStringList spiceSmartcardManager_get_readers();
    bool        spiceSmartcardReader_is_software (QString&);
};

#endif // QSPICE_SMARTCARDMANAGER_H
