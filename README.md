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

Buid requires:

    * with Qt4
    libQtCore.so
    libQtGui.so
    libQtMultimedia.so
    libQtSvg.so
    libQtXml.so
    libqtermwidget4.so

    * with Qt5
    libQt5Core.so
    libQt5Gui.so
    libQt5Multimedia.so
    libQt5Svg.so
    libQt5Widgets.so
    libQt5Xml.so
    libqtermwidget5.so

    libcacard.so            (optional, WITH_LIBCACARD)
    libgio-2.0.so
    libglib-2.0.so
    libgmodule-2.0.so
    libgobject-2.0.so
    libgthread-2.0.so
    libspice-client-glib-2.0.so
    libvirt.so

