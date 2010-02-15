# Build configuration

QMAKE_TARGET_COMPANY = Nokia
QMAKE_TARGET_PRODUCT = DirectUI
QMAKE_TARGET_DESCRIPTION = DirectUI Framework
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2010 Nokia

# Features
# Parts to build. Options: libs tests benchmarks demos doc debian
#DUI_BUILD_PARTS = libs tests benchmarks demos doc debian


contains(TEMPLATE, app) {
    DEFINES += DUI_APPLICATION_NAME=\\\"${QMAKE_TARGET}\\\"
} else {
    contains(TEMPLATE, lib) {
        DEFINES += DUI_LIBRARY_NAME=\\\"lib${QMAKE_TARGET}\\\"
    } else {
        # error(Unknown template)
    }
}

mac {
    DUI_BUILD_FEATURES = debug
    INCLUDEPATH += include
}

# Load configure script results
!win32:!macx {
    include(duiconfig.pri)
}

# Load global definitions
include(../src/dui_defines.prf)

# Defines for directories, for use in source code.
{
    # THEMEDIR determines the location of the theme
    DEFINES += THEMEDIR=\\\"\"$$DUI_THEME_DIR\"\\\"

    # APPLET_LIBS determines the location where all applet binaries are
    DEFINES += APPLET_LIBS=\\\"\"$$DUI_APPLET_DIR\"\\\"

    # APPLET_INSTALLATION_SOURCES determines the location where applet installation source binaries are
    DEFINES += APPLET_INSTALLATION_SOURCES=\\\"\"$$DUI_APPLET_INSTALLATION_SOURCES_DIR\"\\\"

    # APPLET_DATA determines where the .desktop files are located
    DEFINES += APPLET_DATA=\\\"\"$$DUI_APPLET_DATA_DIR\"\\\"

    # APPLET_SETTINGS_DIR determines where the applet global and instance settings files are located
    DEFINES += APPLET_SETTINGS_DIR=\\\"\"$$DUI_APPLET_SETTINGS_DIR\"\\\"

    # TRANSLATION_DIR determines the default translation path
    DEFINES += TRANSLATION_DIR=\\\"\"$$DUI_TRANSLATION_DIR\"\\\"

    # DUI_THEME_PRELOAD_DIR and DUI_THEME_POST_PRELOAD_DIR defines from where
    # to get lists of images to be preloaded
    DEFINES += DUI_THEME_PRELOAD_DIR=\\\"\"$$DUI_THEME_PRELOAD_DIR\"\\\"
    DEFINES += DUI_THEME_POST_PRELOAD_DIR=\\\"\"$$DUI_THEME_POST_PRELOAD_DIR\"\\\"
    DEFINES += DUI_DBUS_SERVICES_DIR=\\\"\"$$DUI_DBUS_SERVICES_DIR\"\\\"
    DEFINES += DUI_XDG_DIR=\\\"\"$$DUI_XDG_DIR\"\\\"

    # DUI_BINARY_SHADERS_DIR defines the location of precompiled shader programs
    DEFINES += DUI_SHADER_SOURCE_DIR=\\\"\"$$DUI_SHADER_SOURCE_DIR\"\\\"
    DEFINES += DUI_SHADER_BINARY_DIR=\\\"\"$$DUI_SHADER_BINARY_DIR\"\\\"
}

# Compiler configuration for all subprojects in libdui

!win32-msvc*:QMAKE_CXXFLAGS += -g

# To use compiler cache, "export USE_CCACHE=true"
USE_CCACHE=$$(USE_CCACHE)
contains(USE_CCACHE, "true") {
    message("Using compiler cache")
    QMAKE_CC = ccache gcc
    QMAKE_CXX = ccache g++
}

include(shared.pri)
