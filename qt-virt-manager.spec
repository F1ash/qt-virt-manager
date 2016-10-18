%global cmake_build_dir build-cmake
%global debug_package   %{nil}
%bcond_without  qt4
%bcond_without  qt5

Name:           qt-virt-manager
Version:        0.37.57
Release:        1%{?dist}
Summary:        Qt Virtual Machine Manager
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz
URL:            http://f1ash.github.io/%{name}

Requires:       libvirt
Requires:       hicolor-icon-theme
%if %with qt4
Requires:       qtermwidget >= 0.6.0-2
Requires:       qt4-remote-viewer
%endif
%if %with qt5
Requires:       qtermwidget-qt5 >= 0.6.0-2
Requires:       qt5-remote-viewer
# for SPICE audio channels
Requires:       qt5-qtmultimedia
%endif
# for use qemu-kvm (more useful)
Requires:       qemu-kvm
Requires:       libvirt-daemon-driver-qemu

# for use SPICE viewer
# spice-server exist in libvirt-daemon-driver-qemu Requires in Fedora
#Requires:      spice-server
Requires:       spice-vdagent

# for use VNC viewer
Requires:       libvncserver

# for scrubbing (optional)
Suggests:       scrub
# netcat for ssh-transported remote connections (optional)
Suggests:       nc
# for use VNC client with tls (optional)
Suggests:       gnutls

%if %with qt4
BuildRequires:  qt4-devel
BuildRequires:  qtermwidget-devel >= 0.6.0-2
%endif
%if %with qt5
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qtsvg-devel
BuildRequires:  qtermwidget-qt5-devel >= 0.6.0-2
BuildRequires:  qt5-qtmultimedia-devel
%endif
BuildRequires:  libvirt-devel
BuildRequires:  glibc-headers
BuildRequires:  desktop-file-utils
BuildRequires:  cmake
BuildRequires:  glib2-devel
BuildRequires:  spice-protocol
BuildRequires:  spice-glib-devel
BuildRequires:  libvncserver-devel
%if %{?fedora}>=24
BuildRequires:  libcacard-devel
%endif

%description
Qt Virtual Machine Manager provides a graphical tool for administering virtual
machines for QEMU/KVM, Xen, OpenVZ, VBox, VMWare and LXC and other Virtual Entities.
Start, stop, add or remove virtual devices, connect to a graphical or serial console,
and see resource usage statistics for existing VMs on local or remote machines.
Uses libvirt as the back-end management API. Uses Spice/VNC viewers for control.

%package -n qt4-virt-manager
Summary:        Qt4 Virtual Machine Manager

%description -n qt4-virt-manager
Qt4 Virtual Machine Manager provides a graphical tool for administering virtual
machines for QEMU/KVM, Xen, OpenVZ, VBox, VMWare and LXC and other Virtual Entities.
Start, stop, add or remove virtual devices, connect to a graphical or serial console,
and see resource usage statistics for existing VMs on local or remote machines.
Uses libvirt as the back-end management API. Uses Spice/VNC viewers for control.

%package -n qt5-virt-manager
Summary:        Qt5 Virtual Machine Manager

%description -n qt5-virt-manager
Qt5 Virtual Machine Manager provides a graphical tool for administering virtual
machines for QEMU/KVM, Xen, OpenVZ, VBox, VMWare and LXC and other Virtual Entities.
Start, stop, add or remove virtual devices, connect to a graphical or serial console,
and see resource usage statistics for existing VMs on local or remote machines.
Uses libvirt as the back-end management API. Uses Spice/VNC viewers for control.

%package -n qt4-remote-viewer
Summary:        Qt4 Remote Viewer

%description -n qt4-virt-manager
Qt4 viewer for remote access to Spice/VNC desktops.

%package -n qt5-remote-viewer
Summary:        Qt5 Remote Viewer

%description -n qt5-virt-manager
Qt5 viewer for remote access to Spice/VNC desktops.

%prep
%setup -q

%build
%if %with qt4
mkdir %{cmake_build_dir}-qt4
pushd %{cmake_build_dir}-qt4
%if %{?fedora}>=24
      %cmake -DWITH_LIBCACARD=1 ..
%else
      %cmake ..
%endif
      %{make_build}
popd
%endif
%if %with qt5
mkdir %{cmake_build_dir}-qt5
pushd %{cmake_build_dir}-qt5
%if %{?fedora}>=24
      %cmake -DBUILD_QT_VERSION=5 -DWITH_LIBCACARD=1 ..
%else
      %cmake -DBUILD_QT_VERSION=5 ..
%endif
      %{make_build}
popd
%endif

%install
%if %with qt4
pushd %{cmake_build_dir}-qt4
      %{make_install}
popd
%endif
%if %with qt5
pushd %{cmake_build_dir}-qt5
      %{make_install}
popd
%endif

%check
%if %with qt4
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt4-virt-manager.desktop
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt4-remote-viewer.desktop
%endif
%if %with qt5
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt5-virt-manager.desktop
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt5-remote-viewer.desktop
%endif

%post
/bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

%postun
if [ $1 -eq 0 ] ; then
    /bin/touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    /usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
/usr/bin/gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%if %with qt4
%files -n qt4-virt-manager
%license LICENSE
%doc README.md Licenses Changelog
%{_bindir}/qt4-virt-manager
%{_datadir}/applications/qt4-virt-manager.desktop
%{_datadir}/qt4-virt-manager
%{_datadir}/icons/hicolor/256x256/apps/virtual-engineering.png

%files -n qt4-remote-viewer
%license LICENSE
%{_bindir}/qt4-remote-viewer
%{_datadir}/applications/qt4-remote-viewer.desktop
%{_datadir}/icons/hicolor/256x256/apps/remote-desktop-viewer.png
%endif

%if %with qt5
%files -n qt5-virt-manager
%license LICENSE
%doc README.md Licenses Changelog
%{_bindir}/qt5-virt-manager
%{_datadir}/applications/qt5-virt-manager.desktop
%{_datadir}/qt5-virt-manager
%{_datadir}/icons/hicolor/256x256/apps/virtual-engineering.png

%files -n qt5-remote-viewer
%license LICENSE
%{_bindir}/qt5-remote-viewer
%{_datadir}/applications/qt5-remote-viewer.desktop
%{_datadir}/icons/hicolor/256x256/apps/remote-desktop-viewer.png
%endif

%changelog
* Tue Oct 18 2016 Fl@sh <kaperang07@gmail.com> - 0.37.57-1
- version updated;
- added changes for qt-remote-viewer subpackages;

* Wed Oct  5 2016 Fl@sh <kaperang07@gmail.com> - 0.37.56-1
- version updated;

* Wed Sep 21 2016 Fl@sh <kaperang07@gmail.com> - 0.36.55-1
- version updated;
- added remote-desktop-viwer into %%check and %%files;

* Sat Jul 16 2016 Fl@sh <kaperang07@gmail.com> - 0.34.50-1
- version updated;

* Wed May 11 2016 Fl@sh <kaperang07@gmail.com> - 0.27.50-1
- version updated;
- clear %%changelog;
