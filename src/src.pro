MOC_DIR = .moc
GEN_DIR = .gen
OBJECTS_DIR = .obj
DESTDIR = ../lib
TEMPLATE = lib
TARGET = dui

include(../mkspecs/common.pri)
include(predeps.pri)

include(animation/animation.pri)
include(core/core.pri)
include(events/events.pri)
include(feedback/feedback.pri)
include(layout/layout.pri)
include(settingslanguage/settingslanguage.pri)
include(painting/painting.pri)
include(scene/scene.pri)
include(service/service.pri)
include(style/style.pri)
include(theme/theme.pri)
include(i18n/i18n.pri)
include(widgets/widgets.pri)
include(workspace/workspace.pri)
# !macx:include(video/video.pri) DISABLED FOR NOW
contains(DEFINES, HAVE_DBUS) {
    include(mashup/appletcommunication/appletcommunication.pri)
    include(mashup/appletinterface/appletinterface.pri)
    include(mashup/appletinstallation/appletinstallation.pri)
    include(mashup/mashup/mashup.pri)
    include(servicefwif/servicefwif.pri)
    include(notification/notification.pri)
}


## Build configuration

VERSION = $${DUI_VERSION}

INCLUDEPATH += include
INCLUDEPATH += $${OUT_PWD}/.moc
INCLUDEPATH += $${OUT_PWD}/.gen

QMAKE_STRIP = echo

!win32 {
    PRE_TARGETDEPS += ../duigen/duigen
}

CONFIG += qt warn_on depend_includepath qmake_cache target_qt dll create_prl
!win32:CONFIG += link_pkgconfig
macx:CONFIG += lib_bundle

win32 {
    QMAKE_MOC = perl $${IN_PWD}\..\duimoc\duimoc
    INCLUDEPATH += .
} else {
    QMAKE_MOC = PATH=../duigen:$$(PATH) $${IN_PWD}/../duimoc/duimoc
}

QMAKE_CFLAGS += -Werror

QMAKE_CXXFLAGS += -DDUI_EXPORTS
!win32-msvc*:QMAKE_CXXFLAGS += -Wno-long-long -pedantic
!win32:QMAKE_CXXFLAGS += -Werror -Wno-variadic-macros -fvisibility=hidden

!win32-msvc*:LIBS += -lm -lX11
macx:LIBS -= -lX11
win32:LIBS -= -lX11

# enable QString optimizations
DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

# Check for mixing of const and non-const iterators, 
# which can cause problems when built with some compilers:
DEFINES += QT_STRICT_ITERATORS


## Features

contains(DUI_BUILD_FEATURES, coverage) {
  QMAKE_CXXFLAGS += --coverage
  QMAKE_LFLAGS += --coverage
}

contains(DUI_BUILD_FEATURES, timestamps) {
  DEFINES += DUI_TIMESTAMP
}

contains(DUI_BUILD_FEATURES, testable) {
  DEFINES += TESTABLE
  HEADERS += ./core/testabilityinterface.h
}

contains(DUI_PROFILE_PARTS, theme) {
  DEFINES += DUI_THEMESYSTEM_PROFILING_SUPPORT
}


## Dependencies

# list pkg-config dependencies here
!macx:PKGCONFIG += xdamage

contains(DEFINES, HAVE_ICU) {
    LIBS += -licui18n -licuuc -licudata
    include(icu-extradata/extradata.pri)
    EXTRADATA_SOURCE=icu-extradata/extradata.txt
}

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    PKGCONFIG += contextsubscriber-1.0
}

contains(DEFINES, HAVE_GSTREAMER) {
    PKGCONFIG += gstreamer-0.10
    # TODO: Is this  necessary?
    LIBS += -lgstbase-0.10 -lgstinterfaces-0.10 -lgstvideo-0.10

    # On Mac, GStreamer can be also in /opt if installed via macports.org or /usr/local if installed from sources
    macx:INCLUDEPATH += /opt/local/include/gstreamer-0.10
}

contains(DEFINES, HAVE_GCONF) {
    PKGCONFIG += gconf-2.0
    # TODO: Why is this necessary? 
    # The PKGCONFIG call above usually causes qmake to link to the correct libraries
    # automatically, but that does not happen if we remove these explicit mentions.
    LIBS += -lgconf-2
}

contains(DEFINES, HAVE_DBUS) {
    QT += dbus
}

contains(DEFINES, HAVE_GLIB) {
    LIBS += -lglib-2.0
}

QT += \
    svg \
    network \
    core \
    gui \
    opengl


## Files and paths

macx {
    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $$system(find include/Dui*) $$HEADERS
    FRAMEWORK_HEADERS.path = Headers
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

target.path = $$DUI_INSTALL_LIBS

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

win32 {
    # for windows we can live with the broken pattern matching
    FORWARDING_HEADERS = include/Dui*
} else {
    # match only the camel case forwarding headers here
    FORWARDING_HEADERS = $$system( find include/Dui* )
}

install_headers.path = $$DUI_INSTALL_HEADERS
install_headers.files = \
    $$FORWARDING_HEADERS \
    $$HEADERS # Remove this after all subdirs define PUBLIC_HEADERS
    # $$PUBLIC_HEADERS

install_pkgconfig.path = $$DUI_INSTALL_LIBS/pkgconfig
install_pkgconfig.files = Dui.pc

install_prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
install_prf.files = dui.prf dui_defines.prf translations.prf

INSTALLS += \
    target \
    install_headers \
    install_pkgconfig

# causes problems when installing on windows with INSTALL_ROOT
!win32 {
    INSTALLS += install_prf
}

QMAKE_EXTRA_TARGETS += check
check.depends = $$DESTDIR/lib$${TARGET}.so.$$VERSION
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$DESTDIR/lib$${TARGET}.so.$$VERSION
check-xml.commands = $$system(true)
QMAKE_CLEAN += \
    *.gcov \
    ./.obj/*.gcno \
    ./.obj/*.gcda \
