# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.25
# 

Name:       libmeegotouch

# >> macros
# << macros

Summary:    MeeGo Touch Framework
Version:    0.30.6
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
URL:        http://meego.gitorious.org/meegotouch/libmeegotouch
Source0:    %{name}-%{version}.tar.bz2
Source1:    libmeegotouch.sh
Source100:  libmeegotouch.yaml
Requires:   meegotouch-theme >= 0.20.79
Requires:   libmeegotouch-l10n-en
Requires:   mthemedaemon
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(QtOpenGL)
BuildRequires:  pkgconfig(maliit-1.0) >= 0.81.3.0.1
BuildRequires:  pkgconfig(contextsubscriber-1.0) >= 0.5.25
BuildRequires:  pkgconfig(gconf-2.0)
BuildRequires:  pkgconfig(gstreamer-0.10)
BuildRequires:  pkgconfig(gstreamer-base-0.10)
BuildRequires:  pkgconfig(gstreamer-plugins-base-0.10)
BuildRequires:  pkgconfig(icu-uc)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(xcomposite)
BuildRequires:  pkgconfig(xdamage)
BuildRequires:  pkgconfig(libngf0)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  doxygen
BuildRequires:  icu
BuildRequires:  libmlocale-devel
BuildRequires:  desktop-file-utils
BuildRequires:  fdupes
Provides:   libdui >= 0.20.0
Obsoletes:   libdui < 0.20.0
Obsoletes:   libmeegotouch <  0.20.89

%description
Qt based MeeGo Touch Framework for developing touch based user interfaces.


%package devel
Summary:    MeeGo Touch Framework development files
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
This package contains the files necessary to develop applications using
the Meego Touch Framework.


%package l10n-ar
Summary:    Arabic (ar) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-ar
This package contains the Arabic translation for the MeeGo Touch Framework.


%package l10n-de
Summary:    German (de) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-de
This package contains the German translation for the MeeGo Touch Framework.


%package l10n-en
Summary:    English (en) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-en
This package contains the English translation for the MeeGo Touch Framework.


%package l10n-fi
Summary:    Finnish (fi) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-fi
This package contains the Finnish translation for the MeeGo Touch Framework.


%package l10n-hu
Summary:    Hungarian (hu) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-hu
This package contains the Hungarian translation for the MeeGo Touch Framework.


%package l10n-ur
Summary:    Urdu (ur) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-ur
This package contains the Urdu translation for the MeeGo Touch Framework.


%package l10n-zhcn
Summary:    Chinese Simplified (zh_CN) MeeGo Touch Framework translation
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description l10n-zhcn
This package contains the Chinese Simplified translation for the MeeGo Touch Framework.


%package benchmarks
Summary:    Benchmark tests
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   meegotouch-demos

%description benchmarks
This package contains benchmark tests for the MeeGo Touch Framework.


%package tests
Summary:    Unit and functional tests
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   gst-plugins-base
Requires:   gst-plugins-good
Requires:   libmeegotouch-l10n-fi

%description tests
This package contains unit and functional tests for the MeeGo Touch Framework.


%package -n meegotouch-demos
Summary:    MeeGo Touch Framework demos
Group:      System/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   meegotouch-demos-l10n

%description -n meegotouch-demos
This package contains demos for the MeeGo Touch Framework.


%package -n meegotouch-demos-l10n
Summary:    MeeGo Touch Framework demos translations
Group:      Development/Languages
Requires:   %{name} = %{version}-%{release}

%description -n meegotouch-demos-l10n
This package contains the demos translations for the MeeGo Touch Framework.


%package -n meegotouch-demos-tests
Summary:    MeeGo Touch Framework demos tests
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires:   meegotouch-demos

%description -n meegotouch-demos-tests
This package contains demos tests for the MeeGo Touch Framework.



%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
unset LD_AS_NEEDED
# >> build pre
./configure \
-prefix %{_prefix} \
-release \
-tests \
-benchmarks \
-testable \
-no-werror \
-maliit \
%ifarch %{arm}
-meegographicssystem
%else
-no-meegographicssystem
%endif

#icu directory according icu version
%define _icudata %(icu-config --icudata)
%define _icuusrdata %(icu-config --icudata | sed 's/icudt/usrdt/g')
make %{?jobs:-j%jobs}
# << build pre



# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
export GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL=1
%qmake_install
# << install pre
mkdir -p %{buildroot}%{_sysconfdir}/profile.d
cp -a %{SOURCE1} %{buildroot}%{_sysconfdir}/profile.d


# >> install post
# Avoid mmoc confusion: ship script on ARM and binary on IA32
%ifarch %{ix86}
rm -f %{buildroot}%{_bindir}/mmoc.pl
%else
rm -f %{buildroot}%{_bindir}/mmoc
%endif
# Remove some not wanted .git files
rm -rf %{buildroot}/%{_libdir}/libmeegotouch-tests/ut_mimagedirectory-samples
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop
%fdupes  %{buildroot}/%{_datadir}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
# >> files
%config %{_sysconfdir}/profile.d/libmeegotouch.sh
%{_bindir}/mservicemapper
%{_libdir}/libmeegotouchcore.so.*
%{_libdir}/libmeegotouchextensions.so.*
%{_libdir}/libmeegotouchsettings.so.*
%{_libdir}/libmeegotouchviews.so.*
%{_libdir}/libmeegotouchpreloader.so.*
%{_libdir}/meegotouch/applets/mappletrunner
%{_libdir}/meegotouch/applicationextensions/mapplicationextensionrunner
%{_datadir}/dbus-1/services/com.nokia.mservicefw.service
%{_datadir}/dbus-1/services/com.nokia.widgetsgallery.service
%{_datadir}/meegotouch/shaders/default.frag
%{_datadir}/meegotouch/shaders/default.vert
%{_datadir}/meegotouch/shaders/yuv1.frag
%{_datadir}/meegotouch/shaders/yuv3.frag
%{_datadir}/meegotouch/targets/*.conf
%{_datadir}/themes/base/meegotouch/mapplettester/style/mapplettester.css
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-babytoy/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-different/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-sluggish/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-sluggish/vibra.ivt
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-space/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-babytoy/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-different/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-sluggish/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-sluggish/vibra.ivt
%{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-space/audio.wav
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/DefaultAvatar.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/contacts
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/grape.jpg
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/list-item-div-bg-selected.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/list-item-div-bg.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/star.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/style/widgetsgallery.css
%{_datadir}/themes/base/meegotouch/widgetsgallery/svg/Widgets-Gallery-Graphics.svg
%{_datadir}/themes/base/meegotouch/widgetsgallery/svg/multimedia-overlays.svg
%{_datadir}/l10n/meegotouch/libmeegotouch.qm
%{_datadir}/l10n/meegotouch/widgetsgallery.qm
# << files

%files devel
%defattr(-,root,root,-)
# >> files devel
%{_bindir}/applicationextensiondemo
%{_bindir}/imgcachegen
%{_bindir}/dui-rename-files
%{_bindir}/dui-rename-symbols
%{_bindir}/m-servicefwgen
%{_bindir}/mapplettester
%{_bindir}/mapplicationextensiontester
%{_bindir}/mgen
%{_bindir}/mmoc*
%{_bindir}/mnotificationstresstest
%{_bindir}/mnotificationtool
%{_bindir}/mcssvalidator
%{_includedir}/meegotouch/M*
%{_includedir}/meegotouch/*.h
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc
%{_datadir}/qt4/mkspecs/features/*.prf
# << files devel

%files l10n-ar
%defattr(-,root,root,-)
# >> files l10n-ar
%{_datadir}/l10n/meegotouch/libmeegotouch_ar.qm
# << files l10n-ar

%files l10n-de
%defattr(-,root,root,-)
# >> files l10n-de
%{_datadir}/l10n/meegotouch/libmeegotouch_de.qm
# << files l10n-de

%files l10n-en
%defattr(-,root,root,-)
# >> files l10n-en
%{_datadir}/l10n/meegotouch/libmeegotouch_en.qm
# << files l10n-en

%files l10n-fi
%defattr(-,root,root,-)
# >> files l10n-fi
%{_datadir}/l10n/meegotouch/libmeegotouch_fi.qm

# << files l10n-fi

%files l10n-hu
%defattr(-,root,root,-)
# >> files l10n-hu
%{_datadir}/l10n/meegotouch/libmeegotouch_hu.qm
# << files l10n-hu

%files l10n-ur
%defattr(-,root,root,-)
# >> files l10n-ur
%{_datadir}/l10n/meegotouch/libmeegotouch_ur.qm
# << files l10n-ur

%files l10n-zhcn
%defattr(-,root,root,-)
# >> files l10n-zhcn
%{_datadir}/l10n/meegotouch/libmeegotouch_zh_CN.qm
# << files l10n-zhcn

%files benchmarks
%defattr(-,root,root,-)
# >> files benchmarks
%{_libdir}/libmeegotouch-benchmarks/mt_*
%{_libdir}/libmeegotouch-benchmarks/pt_*
%{_datadir}/libmeegotouch-benchmarks/tests.xml
# << files benchmarks

%files tests
%defattr(-,root,root,-)
# >> files tests
%{_libdir}/libmeegotouch-tests/ft_*
%{_libdir}/libmeegotouch-tests/glw1.png
%{_libdir}/libmeegotouch-tests/glw2.png
%{_libdir}/libmeegotouch-tests/priorities.conf
%{_libdir}/libmeegotouch-tests/shader1.frag
%{_libdir}/libmeegotouch-tests/shader1.vert
%{_libdir}/libmeegotouch-tests/shader2.frag
%{_libdir}/libmeegotouch-tests/shader2.vert
%{_libdir}/libmeegotouch-tests/svg/meegotouch-speechbubble.svg
%{_libdir}/libmeegotouch-tests/test.ini
%{_libdir}/libmeegotouch-tests/test2.ini
%{_libdir}/libmeegotouch-tests/themedaemon/currentTheme/meegotouch/font-preloading.conf
%{_libdir}/libmeegotouch-tests/ut_*
%{_datadir}/libmeegotouch-tests/tests.xml
# << files tests

%files -n meegotouch-demos
%defattr(-,root,root,-)
# >> files meegotouch-demos
%{_bindir}/widgetsgallery
%{_bindir}/widgetsgallery_accessmanager
%{_libdir}/meegotouch/applicationextensions/libappletinventory-fakeinstallationsource.so
%{_libdir}/meegotouch/applicationextensions/libapplicationextensiondemo-demoextension.so
%{_libdir}/meegotouch/applicationextensions/libapplicationextensiondemo-demoextension2.so
%{_datadir}/applications/widgetsgallery.desktop
%{_datadir}/dbus-1/services/com.nokia.widgetsgallery.service
%{_datadir}/dbus-1/services/com.nokia.widgetsgallery.AccessManager.service
%{_datadir}/meegotouch/applicationextensions/appletinstallationsource.desktop
%{_datadir}/meegotouch/applicationextensions/applicationextensiondemo-demoextension.desktop
%{_datadir}/meegotouch/applicationextensions/applicationextensiondemo-demoextension2.desktop
%{_datadir}/widgetsgallery/core.xml
%{_datadir}/widgetsgallery/data.album
%{_datadir}/widgetsgallery/data.contact
%{_datadir}/widgetsgallery/imtoolbar/exampletoolbar1.xml
%{_datadir}/widgetsgallery/imtoolbar/exampletoolbar2.xml
%{_datadir}/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-cool.svg
%{_datadir}/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-happy.svg
%{_datadir}/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-tongue.svg
%{_datadir}/widgetsgallery/imtoolbar/icons/icon-m-messaging-smiley-wink.svg
%{_datadir}/widgetsgallery/media/avatar-altimeter.png
%{_datadir}/widgetsgallery/media/avatar-butterfly.png
%{_datadir}/widgetsgallery/media/avatar-cat.png
%{_datadir}/widgetsgallery/media/avatar-fly.png
%{_datadir}/widgetsgallery/media/avatar-jets.png
%{_datadir}/widgetsgallery/media/avatar-pig.png
%{_datadir}/widgetsgallery/media/avatar-pigeon.png
%{_datadir}/widgetsgallery/media/avatar-sunset.png
%{_datadir}/widgetsgallery/media/photo_450x450_*.jpg
%{_datadir}/widgetsgallery/timedemo.css
%{_datadir}/themes/base/meegotouch/icons/icon-l-widgetsgallery.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/slider-bg-blue.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/slider-bg-green.png
%{_datadir}/themes/base/meegotouch/widgetsgallery/images/slider-bg-red.png

#0 length file removed
%exclude %{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-babytoy/vibra.ivt
%exclude %{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/press-space/vibra.ivt
%exclude %{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-babytoy/vibra.ivt
%exclude %{_datadir}/themes/base/meegotouch/widgetsgallery/feedbacks/release-space/vibra.ivt
%exclude %{_datadir}/themes/base/meegotouch/widgetsgallery/widgetsgallery.conf
# << files meegotouch-demos

%files -n meegotouch-demos-l10n
%defattr(-,root,root,-)
# >> files meegotouch-demos-l10n
%{_datadir}/l10n/meegotouch/widgetsgallery_*.qm
# << files meegotouch-demos-l10n

%files -n meegotouch-demos-tests
%defattr(-,root,root,-)
# >> files meegotouch-demos-tests
%{_datadir}/meegotouch-demos-widgetsgallery-tests/tests.xml
# << files meegotouch-demos-tests
