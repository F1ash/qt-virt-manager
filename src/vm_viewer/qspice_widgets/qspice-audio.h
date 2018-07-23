#ifndef QSPICE_AUDIO_H
#define QSPICE_AUDIO_H

#if !defined(QSPICE_WIDGET_H_INSIDE) && !defined(QSPICE_WIDGET_COMPILATION)
#warning "Only <qspice-widget.h> can be included directly"
#endif

#include "qspice-object.h"

class QSpiceAudio : public QSpiceObject
{
public:
    explicit QSpiceAudio(
            QObject *parent   = Q_NULLPTR,
            void    *_session = Q_NULLPTR);
    Q_GPROP_POINTER(MainContext, "main-context")
    Q_GPROP_POINTER(Session, "session")

    bool isAssociated();

private:
    bool state;
};

#endif // QSPICE_AUDIO_H
