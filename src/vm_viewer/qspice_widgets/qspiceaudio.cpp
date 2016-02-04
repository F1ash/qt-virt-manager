
#include <spice-audio.h>
#include "qspiceaudio.h"

QSpiceAudio::QSpiceAudio(QObject *parent, void *_session) :
    QSpiceObject(parent)
{
#if USE_SPICE_AUDIO
    // deprecated since 0.8
    // spice_audio_new(session, context, name);
    SpiceSession *session =
            static_cast<SpiceSession*>(_session);
    gobject = spice_audio_get(session, NULL);
    state = (gobject)? true : false;
#else
    Q_UNUSED(_session);
    state = false;
#endif
}

bool QSpiceAudio::isAssociated()
{
    return state;
}
