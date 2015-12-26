
#include <libcacard.h>
#include "qspicehelper.h"
#include "qspicesmartcardmanager.h"

QSpiceSmartcardManager::QSpiceSmartcardManager(QObject *parent) :
    QSpiceObject(parent)
{
    gobject = NULL;
    init();
}

void QSpiceHelper::card_inserted(SpiceSmartcardManager *manager,
                                 SpiceSmartcardReader  *reader,
                                 gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj =
            static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _name;
    VReader *_reader = (VReader*)reader;
    _name.append(vreader_get_name(_reader));
    if (_reader) vreader_free(_reader);
    if (!_name.isEmpty()) obj->cardInserted(_name);
}

void QSpiceHelper::card_removed(SpiceSmartcardManager *manager,
                                SpiceSmartcardReader  *reader,
                                gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj =
            static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _name;
    VReader *_reader = (VReader*)reader;
    _name.append(vreader_get_name(_reader));
    if (_reader) vreader_free(_reader);
    if (!_name.isEmpty()) obj->cardRemoved(_name);
}

void QSpiceHelper::reader_added(SpiceSmartcardManager *manager,
                                SpiceSmartcardReader  *reader,
                                gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj =
            static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _name;
    VReader *_reader = (VReader*)reader;
    _name.append(vreader_get_name(_reader));
    if (_reader) vreader_free(_reader);
    if (!_name.isEmpty()) obj->readerAdded(_name);
}

void QSpiceHelper::reader_removed(SpiceSmartcardManager *manager,
                                  SpiceSmartcardReader  *reader,
                                  gpointer user_data)
{
    Q_UNUSED(manager);
    QSpiceSmartcardManager *obj =
            static_cast<QSpiceSmartcardManager*>(user_data);
    if ( NULL==obj ) return;
    QString _name;
    VReader *_reader = (VReader*)reader;
    _name.append(vreader_get_name(_reader));
    if (_reader) vreader_free(_reader);
    if (!_name.isEmpty()) obj->readerRemoved(_name);
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
        //qDebug()<<"SpiceSmartcardManager initiated";
    } else {
        //qDebug()<<"SpiceSmartcardManager not initiated";
    }
}


QStringList QSpiceSmartcardManager::spiceSmartcardManager_get_readers()
{
    QStringList _readerList;
    GList *_list = spice_smartcard_manager_get_readers(
                (SpiceSmartcardManager*)gobject);
    size_t count = count = g_list_length(_list);
    for ( uint i = 0; i<count; i++ ) {
        VReader *_reader =
                static_cast<VReader*>(g_list_nth_data(_list, i));
        if ( _reader ) {
            _readerList.append(vreader_get_name(_reader));
            vreader_free(_reader);
        };
    };
    if ( _list ) {
        // When no longer needed, the list must be freed
        // after unreferencing its elements with g_boxed_free()
        g_list_free(_list);
    };
    return _readerList;
}

bool QSpiceSmartcardManager::spiceSmartcardReader_is_software (QString &reader)
{
    if ( reader.isEmpty() ) return false;
    VReader *_reader = vreader_get_reader_by_name(
                reader.toUtf8().constData());
    return spice_smartcard_reader_is_software(
                (SpiceSmartcardReader*)_reader);
}
