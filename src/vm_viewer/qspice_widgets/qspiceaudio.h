#ifndef QSPICEAUDIO_H
#define QSPICEAUDIO_H

#include "qspiceobject.h"

class QSpiceAudio : public QSpiceObject
{
public:
    explicit QSpiceAudio(
            QObject *parent   = nullptr,
            void    *_session = nullptr);
    Q_GPROP_POINTER(MainContext, "main-context")
    Q_GPROP_POINTER(Session, "session")

    bool isAssociated();

private:
    bool state;
};

#endif // QSPICEAUDIO_H
