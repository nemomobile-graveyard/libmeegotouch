include(../../../mkspecs/common.pri)

DUIROOT = ../../..
DUILIB = $$DUIROOT/lib
DUISIF = $$DUIROOT/examples/servicefw
DUISIFINCLUDE = $$DUIROOT/examples/servicefw/include
DUISFWINCLUDE = $$DUIROOT/src/servicefwif
TARGET     = user
DESTDIR = $$DUISIF/bin
TEMPLATE   = app

INCLUDEPATH += \
    . \
    $$DUISIFINCLUDE \
    $$DUISFWINCLUDE \
    $$DUISFWINCLUDE/include \
    $$DUISIFINCLUDE \
    $$DUIROOT/src/include \

DEPENDPATH += $$INCLUDEPATH

QMAKE_LIBDIR += \
    $$DUILIB \
    $$DUISIF/lib \

HEADERS    = \
    echowindow.h \

SOURCES    = \
    echowindow.cpp \
    main.cpp \

LIBS += -lduicore

QT += dbus
LIBS += \
    -ltextprocessor \

target.path = $$DUI_INSTALL_BIN

INSTALLS += \
    target
