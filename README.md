qt-virt-manager
=================

A GUI application for managing virtual machines.

Known issues:

0. virsh vol-resize does not shrink volume (see [Red Hat Bugzilla ticket #1021802](https://bugzilla.redhat.com/1021802) for details).

1. You need to create the following symlink in order to use askpass in KDE:

    `$ sudo ln -s /usr/bin/ksshaskpass /usr/libexec/openssh/ssh-askpass`

2. Adding devices implemented partially.
