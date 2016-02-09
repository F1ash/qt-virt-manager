%global cmake_build_dir build-cmake
%global debug_package   %{nil}
%bcond_without  qt4
%bcond_without  qt5

Name:           qt-virt-manager
Version:        0.22.43
Release:        1%{?dist}
Summary:        Qt Virtual Machine Manager
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz
URL:            https://github.com/F1ash/%{name}

Requires:       libvirt
Requires:       hicolor-icon-theme
%if %with qt4
Requires:       qtermwidget >= 0.6.0-2
%endif
%if %with qt5
Requires:       qtermwidget-qt5 >= 0.6.0-2
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
Requires:       xorg-x11-drv-qxl
# for scrubbing (optional)
Requires:       scrub
# for ssh-transported remote connections (optional)
Requires:       nc6

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
%if 0 %{?fedora}>=24
BuildRequires:  libcacard-devel
%endif

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
mkdir %{cmake_build_dir}-qt4
pushd %{cmake_build_dir}-qt4
%if 0 %{?fedora}>=24
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
%if 0 %{?fedora}>=24
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
%endif
%if %with qt5
desktop-file-validate %{buildroot}/%{_datadir}/applications/qt5-virt-manager.desktop
%endif

%if %with qt4
%files -n qt4-virt-manager
%license LICENSE
%doc README.md Licenses Changelog
%{_bindir}/qt4-virt-manager
%{_datadir}/applications/qt4-virt-manager.desktop
%{_datadir}/qt4-virt-manager
%{_datadir}/icons/hicolor/256x256/apps/virtual-engineering.png
%endif
%if %with qt5
%files -n qt5-virt-manager
%license LICENSE
%doc README.md Licenses Changelog
%{_bindir}/qt5-virt-manager
%{_datadir}/applications/qt5-virt-manager.desktop
%{_datadir}/qt5-virt-manager
%{_datadir}/icons/hicolor/256x256/apps/virtual-engineering.png
%endif

%changelog
* Tue Feb 09 2016 Fl@sh <kaperang07@gmail.com> - 0.22.43-1
- added WITH_LIBCACARD build parameter;
- version updated;

* Tue Dec 15 2015 Fl@sh <kaperang07@gmail.com> - 0.22.42-1
- added libcacard-devel BR;
- added qt5-qtmultimedia, hicolor-icon-theme to R;
- set version control for qemu and libcacard-devel;
- version updated;

* Thu Nov  5 2015 Fl@sh <kaperang07@gmail.com> - 0.21.40-1
- erased libcacard-devel BR;
- version updated;

* Sun Sep 20 2015 Fl@sh <kaperang07@gmail.com> - 0.16.38-1
- improved spec for using SPICE, Qemu/KVM, ssh connections;
- version updated;

* Tue Jun 23 2015 Fl@sh <kaperang07@gmail.com> - 0.12.28-1
- version updated;

* Wed May 13 2015 Fl@sh <kaperang07@gmail.com> - 0.10.25-1
- changed %%files for desktop icon;
- version updated;

* Thu Apr 23 2015 Fl@sh <kaperang07@gmail.com> - 0.10.21-1
- version updated;
- clear %%changelog;
