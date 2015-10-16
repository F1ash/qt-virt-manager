#include "qspiceaudio.h"

QSpiceAudio::QSpiceAudio(QObject *parent) :
    QSpiceObject(parent)
{

}

SpiceAudio* QSpiceAudio::spiceAudio_get(SpiceSession *session,
                                        GMainContext *context)
{
    return spice_audio_get(session, context);
}

// deprecated since 0.8
/*
SpiceAudio* QSpiceAudio::spiceAudio_new(SpiceSession *session,
                                        GMainContext *context,
                                        const char *name)
{
    return spice_audio_new(session, context, name);
}
*/
