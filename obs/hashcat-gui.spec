#
# spec file for package hashcat-gui
#

Name:           hashcat-gui
Version:        0.8.0
Release:        0
Summary:        Graphical user interface for hashcat
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
BuildRequires: cmake(Qt6Network)
BuildRequires: cmake(Qt6LinguistTools)
BuildRequires: hicolor-icon-theme
%endif

%if 0%{?debian_version} || 0%{?ubuntu_version}
BuildRequires: g++
BuildRequires: cmake
BuildRequires: qt6-base-dev
BuildRequires: qt6-tools-dev
BuildRequires: qt6-l10n-tools
BuildRequires: debbuild-macros
%endif

%description
A graphical user interface for hashcat, the password recovery utility.

%prep
%setup -q

%build
# BUILD_TESTING keeps the test binaries out of the package build - they
# are run by CI, not by the packager. The translations are not optional
# here: a package without them is a broken package.
%cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DHASHCAT_GUI_REQUIRE_TRANSLATIONS=ON
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
