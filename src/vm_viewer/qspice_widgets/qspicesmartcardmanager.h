#ifndef QSPICESMARTCARDMANAGER_H
#define QSPICESMARTCARDMANAGER_H

#include "qspiceobject.h"
#include <QStringList>

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
    bool        spiceSmartcardReader_is_software (QString&);
};

#endif // QSPICESMARTCARDMANAGER_H
