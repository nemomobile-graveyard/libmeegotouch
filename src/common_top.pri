# this file contains the parts that are common to all libs

MOC_DIR = .moc
GEN_DIR = .gen
OBJECTS_DIR = .obj
DESTDIR = $$M_BUILD_TREE/lib
TEMPLATE = lib

include(../mkspecs/common.pri)
include(predeps.pri)
include(mgen.pri)

VERSION = $${M_VERSION}
DEFINES += M_VERSION=\\\"$${M_VERSION}\\\"

INCLUDEPATH += $${M_SOURCE_TREE}/src/include
INCLUDEPATH += $${OUT_PWD}/.moc
INCLUDEPATH += $${OUT_PWD}/.gen

QMAKE_STRIP = echo

PRE_TARGETDEPS += $$MGEN_EXECUTABLE

CONFIG += qt warn_on depend_includepath qmake_cache target_qt dll create_prl
!win32:CONFIG += link_pkgconfig
macx:CONFIG += lib_bundle

win32 {
    QMAKE_MOC = ..\mmoc\mmoc
    INCLUDEPATH += .
} else {
    QMAKE_MOC = $${M_SOURCE_TREE}/mmoc/mmoc
}

QMAKE_CFLAGS += -Werror

QMAKE_CXXFLAGS += -DM_EXPORTS
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


## Build configuration



## Features

contains(M_BUILD_FEATURES, coverage) {
  QMAKE_CXXFLAGS += --coverage
  QMAKE_LFLAGS += --coverage
}

contains(M_BUILD_FEATURES, timestamps) {
  DEFINES += M_TIMESTAMP
}

contains(M_BUILD_FEATURES, testable) {
  DEFINES += TESTABLE
  HEADERS += ../corelib/core/testabilityinterface.h
}

contains(M_BUILD_FEATURES, pch) {
  DEFINES += USING_PCH
  PRECOMPILED_HEADER = pch/stable_pch.h
  HEADERS += pch/stable_pch.h
  CONFIG += precompile_header
}

contains(M_PROFILE_PARTS, theme) {
  DEFINES += M_THEMESYSTEM_PROFILING_SUPPORT
}


## Dependencies

# list pkg-config dependencies here
!macx:PKGCONFIG += xdamage

contains(DEFINES, HAVE_ICU) {
    LIBS += -licui18n -licuuc -licudata
}

contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    PKGCONFIG += contextsubscriber-1.0
}

contains(DEFINES, HAVE_CONTENTACTION) {
    PKGCONFIG += contentaction-0.1
}

contains(DEFINES, HAVE_GSTREAMER) {
    PKGCONFIG += gstreamer-0.10
    # TODO: Is this  necessary?
    LIBS += -lgstbase-0.10 -lgstinterfaces-0.10 -lgstvideo-0.10

    # On Mac, GStreamer can be also in /opt if installed via macports.org or /usr/local if installed from sources
    macx:INCLUDEPATH += /opt/local/include/gstreamer-0.10 /opt/local/include/glib-2.0 /opt/local/lib/glib-2.0/include/ /opt/local/include/libxml2
    macx:LIBS += -L/opt/local/lib -lgstreamer-0.10.0 -lgstbase-0.10.0 -lgstinterfaces-0.10.0 -lgstvideo-0.10.0 -lglib-2.0.0 -lgobject-2.0.0
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

target.path = $$M_INSTALL_LIBS

