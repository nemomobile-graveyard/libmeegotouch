include(../../../mkspecs/common.pri)

CONFIG += mobility
MOBILITY = serviceframework

MROOT = ../../..
MLIB = $$MROOT/lib
MSIF = $$MROOT/examples/servicefw
MSIFINCLUDE = $$MROOT/examples/servicefw/include
MSFWINCLUDE = $$MROOT/src/servicefwif
TARGET     = qtmuser
DESTDIR = $$MSIF/bin
TEMPLATE   = app

INCLUDEPATH += \
    . \

DEPENDPATH += $$INCLUDEPATH

HEADERS    = \
    echowindow.h \

SOURCES    = \
    echowindow.cpp \
    main.cpp \
