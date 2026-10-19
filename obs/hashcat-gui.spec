#
# spec file for package hashcat-gui
#

Name:           hashcat-gui
Version:        0.7.1
Release:        0
Summary:        Qt-based GUI for hashcat (Password recovery utility)
License:        GPL-3.0-or-later
URL:            https://github.com/rgroesslinger/hashcat-gui/
Source0:        https://github.com/rgroesslinger/hashcat-gui/archive/v%{version}.tar.gz
%if 0%{?debian_version} || 0%{?ubuntu_version}
Packager:       Rainer Größlinger <rainer.groesslinger@gmail.com>
%endif

%if 0%{?fedora_version} || 0%{?suse_version}
BuildRequires: gcc-c++
BuildRequires: cmake
BuildRequires: cmake(Qt6Core)
BuildRequires: cmake(Qt6Gui)
BuildRequires: cmake(Qt6Widgets)
BuildRequires: cmake(Qt6Concurrent)
%endif

%if 0%{?debian_version} || 0%{?ubuntu_version}
BuildRequires: g++
BuildRequires: cmake
BuildRequires: qt6-base-dev
BuildRequires: debbuild-macros
%endif

%description
A Qt-based GUI for hashcat, a powerful password recovery utility

%prep
%setup -q

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%license LICENSE
%doc README.md FAQ.md CHANGELOG.md
%{_bindir}/hashcat-gui
# The desktop entry and the icon tree that install() puts on the system.
%{_datadir}/applications/hashcat-gui.desktop
%{_datadir}/icons/hicolor/*/apps/hashcat-gui.png
%{_datadir}/icons/hicolor/scalable/apps/hashcat-gui.svg

%check

%changelog
