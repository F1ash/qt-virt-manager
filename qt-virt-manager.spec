%global debug_package   %{nil}
%bcond_without  qt4
%bcond_without  qt5

Name:           qt-virt-manager
Version:        0.10.21
Release:        1%{?dist}
Summary:        Qt Virtual Machine Manager
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz 
URL:            https://github.com/F1ash/%{name}

Requires:       libvirt
%if %with qt4
Requires:       qtermwidget >= 0.6.0-2
%endif
%if %with qt5
Requires:       qtermwidget-qt5 >= 0.6.0-2
%endif
# for scrubbing (optional)
Requires:       scrub

%if %with qt4
BuildRequires:  qt4-devel
BuildRequires:  qtermwidget-devel >= 0.6.0-2
%endif
%if %with qt5
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qtsvg-devel
BuildRequires:  qtermwidget-qt5-devel >= 0.6.0-2
%endif
BuildRequires:  libvirt-devel
BuildRequires:  glibc-headers
BuildRequires:  desktop-file-utils
BuildRequires:  meson
BuildRequires:  glib2-devel
BuildRequires:  spice-protocol
BuildRequires:  spice-glib-devel

%description
Qt Virtual Machine Manager provides a graphical tool for administering virtual
machines for QEMU/KVM, Xen, and LXC and other Virtual Entities.
Start, stop, add or remove virtual devices, connect to a graphical or serial console,
and see resource usage statistics for existing VMs on local or remote machines.
Uses libvirt as the backend management API.

%package -n qt4-virt-manager
Summary:        Qt4 Virtual Machine Manager

%description -n qt4-virt-manager
Qt4 Virtual Machine Manager provides a graphical tool for administering virtual
machines for QEMU/KVM, Xen, and LXC and other Virtual Entities.
Start, stop, add or remove virtual devices, connect to a graphical or serial console,
and see resource usage statistics for existing VMs on local or remote machines.
Uses libvirt as the backend management API.

%package -n qt5-virt-manager
Summary:        Qt5 Virtual Machine Manager

%description -n qt5-virt-manager
Qt5 Virtual Machine Manager provides a graphical tool for administering virtual
machines for QEMU/KVM, Xen, and LXC and other Virtual Entities.
Start, stop, add or remove virtual devices, connect to a graphical or serial console,
and see resource usage statistics for existing VMs on local or remote machines.
Uses libvirt as the backend management API.

%prep
%setup -q

%build
%if %with qt4
mkdir build-qt4
pushd build-qt4
      %meson .. -Dbuild_qt_version=4
      ninja-build -v
popd
%endif
%if %with qt5
mkdir build-qt5
pushd build-qt5
      %meson .. -Dbuild_qt_version=5
      ninja-build -v
popd
%endif

%install
%if %with qt4
pushd build-qt4
      DESTDIR=%{buildroot} ninja-build -v install
popd
%endif
%if %with qt5
pushd build-qt5
      DESTDIR=%{buildroot} ninja-build -v install
popd
%endif

%check
%if %with qt4
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt4-virt-manager.desktop
%endif
%if %with qt5
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt5-virt-manager.desktop
%endif

%if %with qt4
%files -n qt4-virt-manager
%doc README.md LICENSE Licenses Changelog
%{_bindir}/qt4-virt-manager
%{_datadir}/applications/qt4-virt-manager.desktop
%{_datadir}/qt4-virt-manager
%endif
%if %with qt5
%files -n qt5-virt-manager
%doc README.md LICENSE Licenses Changelog
%{_bindir}/qt5-virt-manager
%{_datadir}/applications/qt5-virt-manager.desktop
%{_datadir}/qt5-virt-manager
%endif

%changelog
* Thu Apr 23 2015 Fl@sh <kaperang07@gmail.com> - 0.10.21-1
- version updated;
- clear %%changelog;
