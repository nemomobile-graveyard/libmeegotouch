# Build configuration

QMAKE_TARGET_COMPANY = Nokia
QMAKE_TARGET_PRODUCT = DirectUI
QMAKE_TARGET_DESCRIPTION = DirectUI Framework
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2010 Nokia


# Features
# Parts to build. Options: libs tests benchmarks demos doc debian
#M_BUILD_PARTS = libs tests benchmarks demos doc debian

# Load configure script results
!win32: {
    include($${M_BUILD_TREE}/mkspecs/meegotouchconfig.pri)
}

#disable werrors on windows and freemantle (M_OS_MAEMO5 comes from meegotouchconfig.pri)
win32 {
    } else {
        contains( DEFINES, M_OS_MAEMO5 ) {
        } else { 
	    !contains( M_BUILD_FEATURES, no-werror ) {
	        QMAKE_CFLAGS *= -Werror
	        QMAKE_CXXFLAGS *= -Werror
            }
        }
    }

contains(TEMPLATE, app) {
    DEFINES += M_APPLICATION_NAME=\\\"$(QMAKE_TARGET)\\\"
} else {
    contains(TEMPLATE, lib) {
        DEFINES += M_LIBRARY_NAME=\\\"lib$(QMAKE_TARGET)\\\"
    } else {
        # error(Unknown template)
    }
}

mac {
    INCLUDEPATH += include
}

# Load global definitions
include($${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf)

# Defines for directories, for use in source code.
{
    # THEMEDIR determines the location of the theme
    DEFINES += THEMEDIR=\\\"\"$$M_THEME_DIR\"\\\"

    # APPLET_LIBS determines the location where all applet binaries are
    DEFINES += APPLET_LIBS=\\\"\"$$M_APPLET_DIR\"\\\"

    # APPLET_INSTALLATION_SOURCES determines the location where applet installation source binaries are
    DEFINES += APPLET_INSTALLATION_SOURCES=\\\"\"$$M_APPLET_INSTALLATION_SOURCES_DIR\"\\\"

    # APPLET_DATA determines where the .desktop files are located
    DEFINES += APPLET_DATA=\\\"\"$$M_APPLET_DATA_DIR\"\\\"

    # APPLET_SETTINGS_DIR determines where the applet global and instance settings files are located
    DEFINES += APPLET_SETTINGS_DIR=\\\"\"$$M_APPLET_SETTINGS_DIR\"\\\"

    # APPLICATION_EXTENSION_LIBS determines the location of application extension binaries
    DEFINES += APPLICATION_EXTENSION_LIBS=\\\"\"$$M_APPLICATION_EXTENSION_DIR\"\\\"

    # APPLICATION_EXTENSION_DATA_DIR determines the location of application extension .desktop files
    DEFINES += APPLICATION_EXTENSION_DATA_DIR=\\\"\"$$M_APPLICATION_EXTENSION_DATA_DIR\"\\\"

    # TRANSLATION_DIR determines the default translation path
    DEFINES += TRANSLATION_DIR=\\\"\"$$M_TRANSLATION_DIR\"\\\"

    # extra data for libicu to be used be default
    DEFINES += M_ICUEXTRADATA_DIR=\\\"\"$$M_ICUEXTRADATA_DIR\"\\\"

    # M_THEME_PRELOAD_DIR and M_THEME_POST_PRELOAD_DIR defines from where
    # to get lists of images to be preloaded
    DEFINES += M_THEME_PRELOAD_DIR=\\\"\"$$M_THEME_PRELOAD_DIR\"\\\"
    DEFINES += M_THEME_POST_PRELOAD_DIR=\\\"\"$$M_THEME_POST_PRELOAD_DIR\"\\\"
    DEFINES += M_DBUS_SERVICES_DIR=\\\"\"$$M_DBUS_SERVICES_DIR\"\\\"
    DEFINES += M_XDG_DIR=\\\"\"$$M_XDG_DIR\"\\\"
    DEFINES += M_INSTALL_SYSCONFDIR=\\\"\"$$M_INSTALL_SYSCONFDIR\"\\\"

    # M_BINARY_SHADERS_DIR defines the location of precompiled shader programs
    DEFINES += M_SHADER_SOURCE_DIR=\\\"\"$$M_SHADER_SOURCE_DIR\"\\\"
    DEFINES += M_SHADER_BINARY_DIR=\\\"\"$$M_SHADER_BINARY_DIR\"\\\"

    # M_TARGETS_CONF_DIR defines the location of target device config files
    DEFINES += M_TARGETS_CONF_DIR=\\\"\"$$M_TARGETS_CONF_DIR\"\\\"

    # Input method toolbar definition location
    DEFINES += M_IM_TOOLBARS_DIR=\\\"\"$$M_IM_TOOLBARS_DIR\"\\\"

    DEFINES += CACHEDIR=\\\"\"$$M_CACHE_DIR\"\\\"
}

unix {
   # drop unneeded dependencies
   !macx {
      QMAKE_LFLAGS += "-Wl,--as-needed"
   }
}

# Compiler configuration for all subprojects in libm

!win32-msvc*:QMAKE_CXXFLAGS *= -g

# To use compiler cache, "export USE_CCACHE=true"
USE_CCACHE=$$(USE_CCACHE)
contains(USE_CCACHE, "true") {
    message("Using compiler cache")
    QMAKE_CC = ccache gcc
    QMAKE_CXX = ccache g++
}

contains( CONFIG, coverage ) {
    QMAKE_CXXFLAGS *= -ftest-coverage -fprofile-arcs
    LIBS *= -lgcov

    QMAKE_CLEAN *= \
    *.gcda \
    *.gcno \
    *.gcov
}

QMAKE_LIBDIR += $${M_BUILD_TREE}/lib

include(shared.pri)
