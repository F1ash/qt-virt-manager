<meta name="ocs-site-verification" content="ba9d219365f62cbdadc0b356f1be1252" />

[![Qt Virtual machines manager icon](https://f1ash.fedorapeople.org/Pictures/virtual-engineering.png)](#top)


1. [Introduction](#qt-virtual-machines-manager)  
2. [Packages](#packages)
3. [Overview](#overview)
   * 3.1 [Virtual machines viewer](#virtual-machines-viewer)
   * 3.2 [Task Warehouse](#task-warehouse)
   * 3.3 [Create virtual machine](#create-virtual-machine)
   * 3.4 [Create network](#create-network)
   * 3.5 [Create network filter](#create-network-filter)
4. [License](#license)
5. [Donate](#donate)
6. [The Soft Touched mode](#the-soft-touched-mode)

## Qt Virtual machines manager

is the GUI for (remote) control a Virtual machines and entities (aka [`Qt VirtManager`](#overview)). The application is designed for (semi-)professional usage and contain external viewer (aka [`Qt Remote Viewer`](#virtual-machines-viewer)) for accessing to (remote) VMs or desktops.

Current version: 0.60.88

***

![Qt Logo](http://d3hp9ud7yvwzy0.cloudfront.net/wp-content/themes/oneqt/images/favicon.ico.gzip)

Uses [Qt](http://www.qt.io) as framework for GUI.

![Made with libvirt](http://libvirt.org/madeWith.png)

Uses [libvirt](http://libvirt.org/index.html) as the back-end management API for operations with a virtual entities. FYI, this [documentation](https://libvirt.org/format.html) can be useful for the creation of virtual entities.

![SPICE logo](http://www.spice-space.org/images/favicon.png)

Uses [Spice client viewer](http://www.spice-space.org) for graphic consoles
to guest domains (VMs).

![VNC logo]( https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/vnc_logo.png )

Uses [VNC client viewer](https://en.wikipedia.org/wiki/Virtual_Network_Computing) for graphic consoles to guest domains (VMs). [[^]](#top)

***
## Packages
![Fedora logo](https://getfedora.org/static/images/fedora-logotext.png)

Development occurs based on the package database of [Fedora](https://fedoraproject.org).

Applications are present as packages:

 * in repo (Qt5 only): `dnf install qt-virt-manager` or only
 `dnf install qt-remote-viewer`

 * in [FedoraPeople](https://f1ash.fedorapeople.org/qt-virt-manager/)

![OS_X logo](https://upload.wikimedia.org/wikipedia/en/4/45/Mac_Finder_icon_%28OS_X_Yosemite%29.png)

Experimental `HomeBrew` for OS X
  
  * [README](https://github.com/F1ash/homebrew-qt-virt-manager) [![Build Status](https://travis-ci.org/F1ash/qt-virt-manager.svg?branch=master)](https://travis-ci.org/F1ash/qt-virt-manager)

![OS_X pic1]( https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/L7ogDY6.png )

![OS_X pic2]( https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/80zviaa.png )

![OS_X pic3]( https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/TmJPIv2.png )

 [[^]](#top)

***
## Overview

The `Main` window for control state and common operations with connections and related (aka virtual entities) virtual machines (VM), networks, storages, interfaces, secrets, network filters.

![Main widget]( https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot12.png )

 [[^]](#top)

### Virtual machines viewer
The SPICE/VNC `VM Viewer` for graphical control a VMs with support graphic channel.
Note, that there is implemented same external `VM Viewer` (aka `Qt Remote Viewer`).  Therefore at too long usage of manager is recommended to use this external viewer. The external viewer (unlike internal) can't control VM, create snapshots etc., but also can be used for accessing to remote SPICE/VNC desktops.

The internal `VM Viewer`:

![VM Spice Display](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot14.png)

The external `Qt Remote Viewer`:

![VM Spice Display](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot59.png)

The LXC `VM Viewer` for graphical control a lxc-containers.

![LXC Console](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot15.png)

 [[^]](#top)

### Task Warehouse
The `Task Warehouse` window for viewing parameters and states of current tasks.

![Boot in Spice Display](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot24.png)

 [[^]](#top)

### Create virtual machine
The `VM Settings` window for create/edit a virtual machines (domains).

![VM Device Settings](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot18.png)

 [[^]](#top)

### Create network
The `Network Editor` with Assistant mode for trivial networks or Advanced mode for complex and elaborate networks.

Advanced Network editor:

![Network Editor](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot40.png)

![Network Editor](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot41.png)

Network editor Assistant:

![Network Editor](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot42.png)

 [[^]](#top)

### Create network filter
Control Virtual Network Filters aka `NWFilters`.

![NWFilter](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot50.png)

![NWFilter_in_Network_Devices](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot52.png)

Network Filter Editor aka `NWFilter Editor`.

![NWFilter_in_Network_Devices](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot55.png)

![NWFilter_in_Network_Devices](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot56.png)

![NWFilter_in_Network_Devices](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot57.png)

 [[^]](#top)

***
## License

Does not contain platform-specific components.

Licensed under [GNU GPLv2+](http://www.gnu.org/licenses/).

Request the issues & features

occurs in GitHub [Issues page](https://github.com/F1ash/qt-virt-manager/issues) [[^]](#top)

***
## Donate

[![Yandex_money](https://raw.githubusercontent.com/F1ash/qt-virt-manager/master/src/icons/128x128/actions/yandex_money.png)](https://money.yandex.ru/embed/donate.xml?account=410014229313543&quickpay=donate&payment-type-choice=on&mobile-payment-type-choice=on&default-sum=300&targets=Donation+for+develop&target-visibility=on&project-name=QtVirtManager&project-site=http://f1ash.github.io/qt-virt-manager&button-text=01)

![VISA_Wallet](https://raw.githubusercontent.com/F1ash/qt-virt-manager/master/src/icons/128x128/actions/qiwi_wallet_visa.png) Card # : `4890494460088297`

![Bitcoin_Wallet](https://raw.githubusercontent.com/F1ash/qt-virt-manager/master/src/icons/128x128/actions/electrum.png) Wallet : `1KnFdQyxjY2PHEamoYjdwYsK1hcC7SzLEE`

![BitcionCash_Wallet](https://raw.githubusercontent.com/F1ash/qt-virt-manager/master/src/icons/128x128/actions/electron-cash.png) Wallet : `bitcoincash:qr7qepftr5nrl0t8p0t8fpg6cq2c9fycmveyv3fsff`

 [[^]](#top)

***

## The `Soft Touched` mode

An additional view mode for `Main` window controls through mouse actions and shortcuts. The `Dock page` :

![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot27.png)

The `Soft Touched` mode have one base window with various content.
As example, `Connections`:

![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot30.png)

As example, `Virtual machines` (aka Domains):

![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot29.png)

As example, `Secrets`:

![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot28.png)


They can be replaced one for the other using the mouse wheel. Also you can use the graphical pointers for navigation between the docks with virtual entities:


![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot31.png)


![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot32.png)


![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot33.png)

![Soft Touched mode](https://raw.githubusercontent.com/F1ash/qt-virt-manager/gh-pages/images/snapshot34.png)

 [[^]](#top)
