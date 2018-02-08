qt-remote-viewer
=================

A GUI application for view graphics on remote hosts.
Suppotrs VNC/Spice protocols.

About `virt-viewer` [connection file](https://github.com/SPICE/virt-viewer/blob/master/man/remote-viewer.pod#connection-file) `(.vv)`.

Additional key for `virt-viewer` group:

    `ssh-port`      for SSH access to remote host which has VM graphics
    with internal `address:port` or `socket`.

Additional group:

    `[graphics]`    contains keys for internal VM graphics

    `address`       is an address of internal graphics

    `port`          is a port of internal graphics

    `socket`        is a socket for internal graphics

    NOTE: if `socket` is present then `address:port` will be ignored anyway.
