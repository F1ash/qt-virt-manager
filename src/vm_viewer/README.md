qt-remote-viewer
=================

![Qt Remote Viewer]( https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot59.png )

A GUI application for viewing graphics on remote hosts.
VNC/Spice protocols are supports.

## Command line usage

**qt5-remote-viewer** [ URL | Path_to_connection_file ]

**URL**

    protocol://[user@]host[:port][/?extensions]

`protocol`=(spice|vnc)

`extensions` is a string with additional parameters for using VM graphics with internal `address:port`

     [transport=ssh&user=<USER>&]addr=<IP>&port=<NUMBER>[&passwd=<PASSWORD>]

or with internal `socket`

    [transport=ssh&user=<USER>&]socket=/path/to/socket[&passwd=<PASSWORD>]

**Path_to_connection_file**

    qt5-remote-viewer ~/some_folder/example_connection_file.vv

**NOTE**: For access to internal VM graphics uses `ssh-tunnel`.

## Connection file

About `virt-viewer` connection file (aka `*.vv`) read more [here](https://github.com/SPICE/virt-viewer/blob/master/man/remote-viewer.pod#connection-file).

Additional key for **`[virt-viewer]`** group:

**`ssh-port`**      for SSH access to remote host which has VM graphics with internal `address:port` or `socket`.

**NOTE**: if `ssh-port` is present then `port` will be ignored anyway.

Additional group **`[graphics]`** contains keys for VM graphics with internal access:

**`address`**       is an internal address;

**`port`**             is an internal port;

**`socket`**         is an internal socket.

**NOTE**: if `socket` is present then `address:port` will be ignored anyway.

## Examples

    [virt-viewer]
    type=vnc
    host=example.com
    port=45678
    ssh-port=22022
    username=root
    password=%PASSWD

    [graphics]
    address=192.168.1.3
    port=5901

In that example `port` will be ignored.



    [virt-viewer]
    type=spice
    host=example.com
    ssh-port=22022
    username=root
    password=%PASSWD

    [graphics]
    address=192.168.1.3
    port=5900
    socket=/var/lib/libvirt/qemu/domain-3-QEmu-F20-KDE-min/spice.sock

In that example `address:port` will be ignored.

