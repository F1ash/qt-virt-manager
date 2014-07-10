%global cmake_build_dir build-cmake

Name:           qt-virt-manager
Version:        0.1.7
Release:        1%{?dist}
Summary:        Qt Virtual Machine Manager
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz 
URL:            https://github.com/F1ash/%{name}

Requires:       libvirt
# for scrubbing (optional)
Requires:       scrub

BuildRequires:  qt4-devel
BuildRequires:  libvirt-devel
BuildRequires:  glibc-headers
BuildRequires:  desktop-file-utils
BuildRequires:  cmake

%description
Qt Virtual Machine Manager provides a graphical tool for administering virtual
machines for KVM, Xen, and LXC. Start, stop, add or remove virtual devices,
connect to a graphical or serial console, and see resource usage statistics
for existing VMs on local or remote machines. Uses libvirt as the backend
management API.

%prep
%setup -q

%build
mkdir %{cmake_build_dir}
pushd %{cmake_build_dir}
      %cmake ..
      make %{?_smp_mflags}
popd

%install
pushd %{cmake_build_dir}
      %make_install
popd

%check
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%doc README.md LICENSE Licenses Changelog
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/%{name}

%changelog
* Thu Jul 10 2014 Fl@sh <kaperang07@gmail.com> - 0.1.7-1
- version updated;

* Wed May 21 2014 Fl@sh <kaperang07@gmail.com> - 0.0.4-1
- added glibc-headers to BR;

* Fri Apr 25 2014 Fl@sh <kaperang07@gmail.com> - 0.0.3-1
- added scrub to R optionally;

* Wed Apr  2 2014 Fl@sh <kaperang07@gmail.com> - 0.0.2-2
- remove open-xdg R;

* Fri Mar  7 2014 Fl@sh <kaperang07@gmail.com> - 0.0.2-1
- version updated;

* Mon Feb 17 2014 Fl@sh <kaperang07@gmail.com> - 0.0.1-1
- begin build;
