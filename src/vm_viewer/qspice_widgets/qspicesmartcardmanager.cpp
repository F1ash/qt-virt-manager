
#include "qspicehelper.h"
#include <smartcard-manager.h>
#include "qspicesmartcardmanager.h"

QSpiceSmartcardManager::QSpiceSmartcardManager(QObject *parent) :
    QSpiceObject(parent)
{
    gobject = NULL;
    init();
}

void QSpiceHelper::card_inserted(SpiceSmartcardManager *manager, VReader *vreader, gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj = static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _vreader;
    //_vreader.append(vreader_get_name(vreader));
    obj->cardInserted(_vreader);
}

void QSpiceHelper::card_removed(SpiceSmartcardManager *manager, VReader *vreader, gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj = static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _vreader;
    //_vreader.append(vreader_get_name(vreader));
    obj->cardRemoved(_vreader);
}

void QSpiceHelper::reader_added(SpiceSmartcardManager *manager, VReader *vreader, gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj = static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _vreader;
    //_vreader.append(vreader_get_name(vreader));
    obj->readerAdded(_vreader);
}

void QSpiceHelper::reader_removed(SpiceSmartcardManager *manager, VReader *vreader, gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj = static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _vreader;
    //_vreader.append(vreader_get_name(vreader));
    obj->readerRemoved(_vreader);
}

void QSpiceSmartcardManager::init()
{
    gobject = spice_smartcard_manager_get();
    if ( gobject ) {
        g_signal_connect(gobject, "card-inserted",
                         (GCallback) QSpiceHelper::card_inserted, this);
        g_signal_connect(gobject, "card-removed",
                         (GCallback) QSpiceHelper::card_removed, this);
        g_signal_connect(gobject, "reader-added",
                         (GCallback) QSpiceHelper::reader_added, this);
        g_signal_connect(gobject, "reader-removed",
                         (GCallback) QSpiceHelper::reader_removed, this);
        qDebug()<<"SpiceSmartcardManager initiated";
    } else {
        qDebug()<<"SpiceSmartcardManager not initiated";
    }
}


QStringList QSpiceSmartcardManager::spiceSmartcardManager_get_readers()
{
    QStringList _cardList;
    GList *_list = spice_smartcard_manager_get_readers(
                (SpiceSmartcardManager*)gobject);
    size_t count = g_list_length(_list);
    for ( uint i = 0; i<count; i++ ) {
        //_cardList.append();
    };
    if ( _list ) {
        // When no longer needed, the list must be freed
        // after unreferencing its elements with g_boxed_free()
        g_free(_list);
    };
    return _cardList;
}

bool QSpiceSmartcardManager::spiceSmartcardManager_insert_card()
{
    return spice_smartcard_manager_insert_card(
                (SpiceSmartcardManager*)gobject);
}

bool QSpiceSmartcardManager::spiceSmartcardManager_remove_card()
{
    return spice_smartcard_manager_remove_card(
                (SpiceSmartcardManager*)gobject);
}

bool QSpiceSmartcardManager::spiceSmartcardReader_is_software (void *reader)
{
    return spice_smartcard_reader_is_software(
                (SpiceSmartcardReader*)reader);
}

bool QSpiceSmartcardManager::spiceSmartcardReader_insert_card (void *reader)
{
    return spice_smartcard_reader_insert_card(
                (SpiceSmartcardReader*)reader);
}

bool QSpiceSmartcardManager::spiceSmartcardReader_remove_card (void *reader)
{
    return spice_smartcard_reader_remove_card(
                (SpiceSmartcardReader*)reader);
}
