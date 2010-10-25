include(../../../mkspecs/common.pri)

MROOT = ../../..
MLIB = $$MROOT/lib
MSIF = $$MROOT/examples/servicefw
MSIFINCLUDE = $$MROOT/examples/servicefw/include
MSFWINCLUDE = $$MROOT/src/servicefwif
TARGET     = user
DESTDIR = $$MSIF/bin
TEMPLATE   = app

INCLUDEPATH += \
    . \
    $$MSIFINCLUDE \
    $$MSFWINCLUDE \
    $$MSFWINCLUDE/include \
    $$MSIFINCLUDE \
    $$MROOT/src/include \

DEPENDPATH += $$INCLUDEPATH

QMAKE_LIBDIR += \
    $$MLIB \
    $$MSIF/lib \

HEADERS    = \
    echowindow.h \

SOURCES    = \
    echowindow.cpp \
    main.cpp \

LIBS += $$mAddLibrary(meegotouchcore)
LIBS += \
    -ltextprocessor

target.path = $$M_INSTALL_BIN

INSTALLS += \
    target
