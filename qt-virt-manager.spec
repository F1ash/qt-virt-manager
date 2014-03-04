%global cmake_build_dir build-cmake

Name:           qt-virt-manager
Version:        0.0.1
Release:        1%{?dist}
Summary:        Qt Virtual Machine Manager
Group:          Applications/System
License:        GPLv2+
Source0:        https://github.com/F1ash/%{name}/archive/%{version}.tar.gz 
URL:            https://github.com/F1ash/%{name}

Requires:       libvirt
Requires:       sandbox-runner-data >= 0.3.5
# for xdg-open
Requires:       xdg-utils
BuildRequires:  qt4-devel
BuildRequires:  libvirt-devel
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
mkdir %{cmake_build_dir}

%build
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
%doc README.md COPYING Changelog
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop

%changelog
* Mon Feb 17 2014 Fl@sh <kaperang07@gmail.com> - 0.0.1-1
- begin build;
