qt-virt-manager
=================

A GUI application for managing virtual machines.

Known issues:

    1. You need to create the following symlink in order to use askpass in KDE:

        `$ sudo ln -s /usr/bin/ksshaskpass /usr/libexec/openssh/ssh-askpass`

    2. Adding devices implemented partially.

Building parameters:

    WITH_LIBCACARD
        if defined and above zero, then libcacard.h required
        and will used SpiceSmartcard Channel for smartcards

    USE_SPICE_AUDIO
        if not defined or above zero, then will used
        Spice[Playback/Record] Channels for audio

