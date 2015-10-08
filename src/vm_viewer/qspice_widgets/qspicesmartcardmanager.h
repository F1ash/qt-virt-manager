#ifndef QSPICESMARTCARDMANAGER_H
#define QSPICESMARTCARDMANAGER_H

#include "qspiceobject.h"
#include <QStringList>
#include <QDebug>

class QSpiceSmartcardManager : public QSpiceObject
{
    Q_OBJECT
public:
    explicit QSpiceSmartcardManager(QObject *parent = 0);

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
    bool        spiceSmartcardManager_insert_card();
    bool        spiceSmartcardManager_remove_card();
    bool        spiceSmartcardReader_is_software (void*);
    bool        spiceSmartcardReader_insert_card (void*);
    bool        spiceSmartcardReader_remove_card (void*);
};

#endif // QSPICESMARTCARDMANAGER_H
