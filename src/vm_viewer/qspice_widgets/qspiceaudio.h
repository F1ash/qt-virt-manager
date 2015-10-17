#ifndef QSPICEAUDIO_H
#define QSPICEAUDIO_H

#include <spice-audio.h>
#include "qspiceobject.h"

class QSpiceAudio : public QSpiceObject
{
public:
    explicit QSpiceAudio(QObject *parent = NULL);
    Q_GPROP_POINTER(MainContext, "main-context")
    Q_GPROP_POINTER(Session, "session")

    SpiceAudio *spiceAudio_get(SpiceSession *session,
                               GMainContext *context);
    // deprecated since 0.8
    /*
    SpiceAudio *spiceAudio_new(SpiceSession *session,
                               GMainContext *context,
                               const char *name);
     */

signals:

public slots:
};

#endif // QSPICEAUDIO_H
