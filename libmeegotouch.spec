#
# spec file for creating RPM packages from libmeegotouch for MeeGo
#
# See also file README.specfile
#

Name:           libmeegotouch
Version:        0.20.2
Release:        1%{?dist}
Summary:        Meego Touch Framework

Group:          System Environment/Libraries
License:        LGPL v2.1
URL:            http://qt.gitorious.org/maemo-6-ui-framework/libdui

Source0:        %{name}-%{version}.tar.bz2
### Patch1:         0001-Changes-Read-target-device-from-GConf.patch
### Patch2:         0002-Fix-Ensure-rowsRemoved-updates-visible-cells.patch
### Patch3:         0003-Enable-building-of-the-video-widget.patch
### Patch4:         0004-Fix-Calculate-rotation-and-position-per-device-prof.patch
### Patch5:         0005-Add-respect-of-cell-s-preferred-width-in-duifastlist.patch
### Patch6:         0006-Adding-various-Moorestown-and-Menlow-devices-to-the.patch
### Patch7:         0007-Work-around-fix-to-duimoc-s-regexp-matching-to-gener.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)


BuildRequires: pkgconfig(QtCore) >= 4.6.0
BuildRequires: pkgconfig(QtDBus)
BuildRequires: pkgconfig(QtOpenGL)
BuildRequires: pkgconfig(QtMultimedia)
BuildRequires: pkgconfig(QtNetwork)
BuildRequires: pkgconfig(QtGui)
BuildRequires: pkgconfig(gstreamer-0.10)
BuildRequires: pkgconfig(gstreamer-base-0.10)
BuildRequires: pkgconfig(gstreamer-plugins-base-0.10)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(xdamage)
BuildRequires: pkgconfig(icu)
BuildRequires: pkgconfig(x11)
BuildRequires: pkgconfig(gconf-2.0)
BuildRequires: pkgconfig(contextprovider-1.0)
BuildRequires: pkgconfig(gl)
BuildRequires: mesa-libEGL-devel
BuildRequires: qt-devel
BuildRequires: doxygen
BuildRequires: icu

Requires(post):   /sbin/ldconfig
Requires(postun): /sbin/ldconfig


# TO DO: Check if the following makes any sense
# Requires: %{name} >= %{version}

Requires: meegotouch-theme         >= %{version}
Requires: libmeegotouch-l10n
# Requires: libmeegotouch-l10n-eng-en  = %{version}

# The main package is just a wrapper for the lib subpackages
Requires: libmeegotouchcore0       >= %{version}
Requires: libmeegotouchextensions0 >= %{version}
Requires: libmeegotouchsettings0   >= %{version}
Requires: libmeegotouchviews0      >= %{version}
Requires: libmeegotouch-bin        >= %{version}
Requires: meegotouch-qt-style      >= %{version}


#
# Main package
#

%description
Qt based framework for developing touch based user interfaces


#
# Lib and binary subpackages
#
# %package -n foo is just "foo", not <pkg-base-name>-foo (i.e. "meegotouch-foo")
#

%package -n libmeegotouchcore0
Summary: Meego Touch core library
%description -n libmeegotouchcore0
Meego Touch core library


%package -n libmeegotouchextensions0
Summary: Meego Touch extensions library
%description -n libmeegotouchextensions0
Meego Touch extensions library


%package -n libmeegotouchsettings0
Summary: Meego Touch settings library
%description -n libmeegotouchsettings0
Meego Touch settings library


%package -n libmeegotouchviews0
Summary: Meego Touch widget views library
%description -n libmeegotouchviews0
Meego Touch widget views library


%package -n libmeegotouch-bin
Summary: Meego Touch binary files
Requires: libmeegotouchcore0       >= %{version}
Requires: libmeegotouchextensions0 >= %{version}
Requires: libmeegotouchsettings0   >= %{version}
Requires: libmeegotouchviews0      >= %{version}
%description -n libmeegotouch-bin
Meego Touch framework binary files


%package -n meegotouch-qt-style
Summary: Meego Touch plain Qt style
Requires: libmeegotouchcore0       >= %{version}
Requires: libmeegotouchviews0      >= %{version}
%description -n meegotouch-qt-style
Meego Touch Qt style for applications that do not link against libmeegotouch


#
# Development subpackages
#

# -devel is the naming convention for development packages in RPM based distributions.
# Debian uses -dev.
%package -n libmeegotouch-devel
Summary: Meego Touch development files
Group: Development/Libraries
Requires: libmeegotouchcore0       >= %{version}
Requires: libmeegotouchextensions0 >= %{version}
Requires: libmeegotouchsettings0   >= %{version}
Requires: libmeegotouchviews0      >= %{version}
Requires: meegotouch-qt-style     >= %{version}
%description -n libmeegotouch-devel
Development files for Meego Touch


%package -n meegotouch-devel-tools
Summary: Meego Touch development and testing tools
Group: Development/Libraries
Requires: libmeegotouch-devel
%description -n meegotouch-devel-tools
Collection of tools for testing and developing on top of libmeegotouch.
Includes stuff like applettester and code/mock generation.


%package -n libmeegotouch-doc
Summary: Meego Touch API documentation
%description -n libmeegotouch-doc
Meego Touch API documentation


#
# Demo and test subpackages
#

%package -n meegotouch-demos
Summary: Meego Touch demo applications
Requires: meegotouch-demos-widgetsgallery
Requires: meegotouch-demos-qt-style
Requires: meegotouch-demos-animatedlayout
%description -n meegotouch-demos
Collection of demo applications for Meego Touch


%package -n meegotouch-demos-widgetsgallery
Summary: Meego Touch widgets gallery demo
Requires: meegotouch-demos-widgetsgallery-l10n
%description -n meegotouch-demos-widgetsgallery
Meego Touch demo application that shows the available widgets


%package -n meegotouch-demos-widgetsgallery-tests
Summary: Meego Touch widgets gallery tests
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-tests
Meego Touch demo application tests


%package -n meegotouch-demos-qt-style
Summary: Meego Touch plain Qt applications demos
Requires: meegotouch-qt-style
%description -n meegotouch-demos-qt-style
Demo applications that show how plain Qt applications
(applications that do not link against libmeegotouch) are supported
in the Meego Touch framework


%package -n meegotouch-demos-animatedlayout
Summary: Meego Touch animated layout demo
Requires: meegotouch-demos-animatedlayout-l10n
%description -n meegotouch-demos-animatedlayout
Demo application for the Meego Touch animated demo


%package -n meegotouch-demos-appletinstallationsource
Summary: Meego Touch applet installation source demos
%description -n meegotouch-demos-appletinstallationsource
Demo applet installation source for the Meego Touch applet library


%package -n meegotouch-demos-applicationextension
Summary: Meego Touch application extension demos
%description -n meegotouch-demos-applicationextension
Demo application for the Meego Touch application extensions


%package -n libmeegotouch-tests
Summary: Meego Touch unit tests
%description -n libmeegotouch-tests
Unit testing binaries and shell scripts for testing the libmeegotouch library


%package -n libmeegotouch-benchmarks
Summary: Meego Touch benchmarks
%description -n libmeegotouch-benchmarks
Benchmarking binaries for testing the libmeegotouch library


#
# i10n subpackages
#
# -engineering-english had to be shortened to -eng-en because of file name lenght limitations:
# rpmlint complains about exceeding the 64 character file name component limit on Joliet file systems.

%package -n libmeegotouch-l10n-eng-en
Summary: Meego Touch engineering English messages
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-eng-en
Meego Touch engineering English messages


%package -n libmeegotouch-l10n-ar
Summary: Meego Touch Arabic translations
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-ar
Meego Touch Arabic translations


%package -n libmeegotouch-l10n-de
Summary: Meego Touch German translations
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-de
Meego Touch German translations


%package -n libmeegotouch-l10n-en
Summary: Meego Touch English messages
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-en
Meego Touch English messages


%package -n libmeegotouch-l10n-fi
Summary: Meego Touch Finnish translations
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-fi
Meego Touch Finnish translations


%package -n libmeegotouch-l10n-hu
Summary: Meego Touch Hungarian translations
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-hu
Meego Touch Hungarian translations


%package -n libmeegotouch-l10n-ur
Summary: Meego Touch Urdu translations
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-ur
Meego Touch Urdu translations


%package -n libmeegotouch-l10n-zh-cn
Summary: Meego Touch simplified Chinese translations
Provides: libmeegotouch-l10n
%description -n libmeegotouch-l10n-zh-cn
Meego Touch simplified Chinese translations


# i10n widget gallery demo subpackages

%package -n meegotouch-demos-widgetsgallery-l10n-eng-en
Summary: Meego Touch widgets gallery engineering English messages
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-eng-en
Meego Touch widgets gallery engineering English messages


%package -n meegotouch-demos-widgetsgallery-l10n-ar
Summary: Meego Touch widgets gallery Arabic translations
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-ar
Meego Touch widgets gallery Arabic translations


%package -n meegotouch-demos-widgetsgallery-l10n-de
Summary: Meego Touch widgets gallery German translations
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-de
Meego Touch widgets gallery German translations


%package -n meegotouch-demos-widgetsgallery-l10n-en
Summary: Meego Touch widgets gallery English messages
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-en
Meego Touch widgets gallery English messages


%package -n meegotouch-demos-widgetsgallery-l10n-fi
Summary: Meego Touch widgets gallery Finnish translations
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-fi
Meego Touch widgets gallery Finnish translations


%package -n meegotouch-demos-widgetsgallery-l10n-hu
Summary: Meego Touch widgets gallery Hungarian translations
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-hu
Meego Touch widgets gallery Hungarian translations

%package -n meegotouch-demos-widgetsgallery-l10n-ur
Summary: Meego Touch widgets gallery Urdu translations
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-ur
Meego Touch widgets gallery Urdu translations


%package -n meegotouch-demos-widgetsgallery-l10n-zh-cn
Summary: Meego Touch widgets gallery simplified Chinese translations
Provides: meegotouch-demos-widgetsgallery-l10n
Requires: meegotouch-demos-widgetsgallery
%description -n meegotouch-demos-widgetsgallery-l10n-zh-cn
Meego Touch widgets gallery simplified Chinese translations


# i10n animated layout demo subpackages

%package -n meegotouch-demos-animatedlayout-l10n-eng-en
Summary: Meego Touch animated layout demo engineering English messages
Provides: meegotouch-demos-animatedlayout-l10n
%description -n meegotouch-demos-animatedlayout-l10n-eng-en
Meego Touch animated layout engineering English messages


%package -n meegotouch-demos-animatedlayout-l10n-de
Summary: Meego Touch animated layout demo German translations
Provides: meegotouch-demos-animatedlayout-l10n
%description -n meegotouch-demos-animatedlayout-l10n-de
Meego Touch animated layout German translations


%package -n meegotouch-demos-animatedlayout-l10n-en
Summary: Meego Touch animated layout demo English messages
Provides: meegotouch-demos-animatedlayout-l10n
%description -n meegotouch-demos-animatedlayout-l10n-en
Meego Touch animated layout English messages


%package -n meegotouch-demos-animatedlayout-l10n-ja
Summary: Meego Touch animated layout demo Japanese translations
Provides: meegotouch-demos-animatedlayout-l10n
%description -n meegotouch-demos-animatedlayout-l10n-ja
Meego Touch animated layout Japanese translations



# Typically /usr/share/qt4, but might also be /usr/lib/qt4
%define qt_install_data_dir %(qmake -query QT_INSTALL_DATA)


#
# Unpack / build / install rules
#

%prep
%setup -q -n %{name}-%{version}
### %patch1 -p1 -b .p1
### %patch2 -p1 -b .p2
### %patch3 -p1 -b .p3
### %patch4 -p1 -b .p4
### %patch5 -p1 -b .p5
### %patch6 -p1 -b .p6
### %patch7 -p1 -b .p7


%build
export PATH=$PATH:%{_libdir}/qt4/bin
unset LD_AS_NEEDED
./configure -prefix /usr -release -plainqt -tests -benchmarks
make %{?_smp_mflags}


%install
rm -fR %{buildroot}

export GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL=1
make INSTALL_ROOT=%{buildroot} install

# The one file the main package contains
# (rpmlint complains if it doesn't have any file at all)
install -m 644 -D -p LICENSE.LGPL %{buildroot}/usr/share/doc/packages/%{name}/LICENSE.LGPL

# FIXME find out what this libdui.sh is good for (not packaged anywhere yet)
### mkdir -p %{buildroot}/etc/profile.d
### echo "export DUI_DECORATED=0" > %{buildroot}/etc/profile.d/libdui.sh

# FIXME the next 2 lines should go to the Makefile
install -m 644 -D -p demos/widgetsgallery/translations/widgetsgallery.ts %{buildroot}/usr/share/doc/meegotouch-demos-widgetsgallery-l10n-eng-en/widgetsgallery.ts
install -m 644 -D -p src/translations/libmeegotouch.ts                   %{buildroot}/usr/share/doc/libmeegotouch-l10n-eng-en/libmeegotouch.ts


%clean
rm -fR %{buildroot}


%post  -n libmeegotouchcore0
/sbin/ldconfig
# create cache directory
mkdir -p -m a+w /var/cache/meegotouch
mkdir -p -m a+w /var/cache/meegotouch/css


%postun -n libmeegotouchcore0
/sbin/ldconfig
# remove cached data
if [ -d /var/cache/meegotouch ]; then
  rm -rf /var/cache/meegotouch
fi


%post -n libmeegotouchextensions0
/sbin/ldconfig

%postun -n libmeegotouchextensions0
/sbin/ldconfig



%post -n libmeegotouchsettings0
/sbin/ldconfig

%postun -n libmeegotouchsettings0
/sbin/ldconfig



%post -n libmeegotouchviews0
/sbin/ldconfig

%postun -n libmeegotouchviews0
/sbin/ldconfig



%post -n libmeegotouch-tests
# add en_US.UTF-8 locale if not already there:
# (this locale is needed for the test ft_meegotouchgconfitem)
if ! egrep -q -E "^[[:space:]]*en_US.UTF-8[[:space:]]+UTF-8" /etc/locale.gen ; then
   echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen
   /usr/sbin/locale-gen
fi


### TO DO: Find out if this is necessary - the Debian packages do it like this
### %post -n libmeegotouch-bin
### GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source` gconftool-2 --makefile-install-rule /etc/gconf/schemas/meegotouch.schemas > /dev/null

### %postun -n libmeegotouch-bin
### GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source` gconftool-2 --makefile-uninstall-rule /etc/gconf/schemas/meegotouch.schemas > /dev/null



#
# File lists
#

%files
%defattr(-,root,root)
%dir /usr/share/doc/packages/%{name}
%doc /usr/share/doc/packages/%{name}/*


%files -n libmeegotouchcore0
%defattr(-,root,root)
%{_libdir}/libmeegotouchcore.so.0*


%files -n libmeegotouchextensions0
%defattr(-,root,root)
%{_libdir}/libmeegotouchextensions.so.0*


%files -n libmeegotouchsettings0
%defattr(-,root,root)
%{_libdir}/libmeegotouchsettings.so.0*


%files -n libmeegotouchviews0
%defattr(-,root,root)
%{_libdir}/libmeegotouchviews.so.0*


%files -n meegotouch-qt-style
%defattr(-,root,root)
%{_libdir}/qt4/plugins/styles/libmeegotouchqtstyleplugin.so


%files -n libmeegotouch-bin
%defattr(-,root,root)
%dir %{_libdir}/meegotouch
%dir %{_libdir}/meegotouch/applets
/usr/bin/mthemedaemon
/usr/bin/mservicemapper
/usr/share/dbus-1/services/com.nokia.mservicefw.service
/usr/share/meegotouch/shaders/*
%{_libdir}/meegotouch/applets/mappletrunner
%config /etc/meegotouch/devices.conf
%config /etc/gconf/schemas/meegotouch.schemas


%files -n libmeegotouch-devel
%defattr(-,root,root)
%dir /usr/include/meegotouch
/usr/include/meegotouch/*
%{_libdir}/libmeegotouch*.so
%{_libdir}/libmeegotouch*.prl
%{_libdir}/pkgconfig/*
%{qt_install_data_dir}/mkspecs/features/meegotouch.prf
%{qt_install_data_dir}/mkspecs/features/meegotouch_defines.prf
%{qt_install_data_dir}/mkspecs/features/meegotouch_translations.prf


%files -n meegotouch-devel-tools
%defattr(-,root,root)
/usr/bin/mmoc
/usr/bin/mgen
/usr/bin/m-servicefwgen
/usr/bin/mapplettester
/usr/bin/mapplicationextensiontester
/usr/bin/mnotificationtool
/usr/bin/mnotificationstresstest
/usr/bin/dui-rename-files
/usr/bin/dui-rename-symbols


%files -n libmeegotouch-doc
%defattr(-,root,root)
%dir /usr/share/doc/libmeegotouch
%doc /usr/share/doc/libmeegotouch/*


%files -n libmeegotouch-tests
%defattr(-,root,root)
%dir %{_libdir}/libmeegotouch-tests
%dir /usr/share/libmeegotouch-tests
/usr/share/libmeegotouch-tests/*
%{_libdir}/libmeegotouch-tests/*


%files -n libmeegotouch-benchmarks
%defattr(-,root,root)
%dir %{_libdir}/libmeegotouch-benchmarks
%{_libdir}/libmeegotouch-benchmarks/*


%files -n meegotouch-demos-qt-style
%defattr(-,root,root)
%dir /usr/share/qtstyleexample
%dir /usr/share/qtstyleexample/themes
/usr/bin/qtstyle*
/usr/share/applications/qtstyle*
/usr/share/dbus-1/services/com.nokia.qtstyle*
/usr/share/qtstyleexample/themes/*


%files -n meegotouch-demos-animatedlayout
%defattr(-,root,root)
%dir /usr/share/animatedlayout
/usr/bin/animatedlayout
/usr/share/animatedlayout/*


%files -n meegotouch-demos-appletinstallationsource
%defattr(-,root,root)
/usr/share/meegotouch/applicationextensions/*installationsource*
%{_libdir}/meegotouch/applicationextensions/*installationsource*


%files -n meegotouch-demos-applicationextension
%defattr(-,root,root)
%dir /usr/share/meegotouch
/usr/bin/applicationextension*
%{_libdir}/meegotouch/applicationextensions/*extension*
/usr/share/meegotouch/applicationextensions/*extension*


%files -n meegotouch-demos-widgetsgallery
%defattr(-,root,root)
%dir /usr/share/widgetsgallery
/usr/bin/widgetsgallery
/usr/share/themes/*
/usr/share/applications/widgetsgallery.desktop
/usr/share/dbus-1/services/com.nokia.widgetsgallery.service
/usr/share/widgetsgallery/*


%files -n meegotouch-demos-widgetsgallery-tests
%defattr(-,root,root)
%dir /usr/share/meegotouch-demos-widgetsgallery-tests
/usr/share/meegotouch-demos-widgetsgallery-tests/tests.xml


%files -n libmeegotouch-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
%dir /usr/share/doc/libmeegotouch-l10n-eng-en
/usr/share/l10n/meegotouch/libmeegotouch.qm
/usr/share/doc/libmeegotouch-l10n-eng-en/libmeegotouch.ts


%files -n libmeegotouch-l10n-ar
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_ar.qm


%files -n libmeegotouch-l10n-de
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_de.qm


%files -n libmeegotouch-l10n-en
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_en.qm


%files -n libmeegotouch-l10n-fi
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_fi.qm


%files -n libmeegotouch-l10n-hu
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_hu.qm


%files -n libmeegotouch-l10n-ur
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_ur.qm


%files -n libmeegotouch-l10n-zh-cn
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouch_zh_CN.qm


%files -n meegotouch-demos-widgetsgallery-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
%dir /usr/share/doc/meegotouch-demos-widgetsgallery-l10n-eng-en
/usr/share/l10n/meegotouch/widgetsgallery.qm
/usr/share/doc/meegotouch-demos-widgetsgallery-l10n-eng-en/widgetsgallery.ts


%files -n meegotouch-demos-widgetsgallery-l10n-ar
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_ar.qm


%files -n meegotouch-demos-widgetsgallery-l10n-de
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_de.qm


%files -n meegotouch-demos-widgetsgallery-l10n-en
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_en.qm


%files -n meegotouch-demos-widgetsgallery-l10n-fi
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_fi.qm


%files -n meegotouch-demos-widgetsgallery-l10n-hu
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_hu.qm


%files -n meegotouch-demos-widgetsgallery-l10n-ur
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_ur.qm


%files -n meegotouch-demos-widgetsgallery-l10n-zh-cn
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/widgetsgallery_zh_CN.qm


%files -n meegotouch-demos-animatedlayout-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/animatedlayout.qm


%files -n meegotouch-demos-animatedlayout-l10n-de
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/animatedlayout_de.qm


%files -n meegotouch-demos-animatedlayout-l10n-en
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/animatedlayout_en.qm


%files -n meegotouch-demos-animatedlayout-l10n-ja
%defattr(-,root,root)
%dir /usr/share/l10n
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/animatedlayout_ja.qm


%changelog
