DUIROOT=../../../../..
include($$DUIROOT/mkspecs/common.pri)

DESTDIR=$$DUIROOT/bin/
TARGET=org.maemo.textprocessor
TEMPLATE=app
SOURCES += \
    main.cpp \
    textprocessor.cpp \
    textprocessorifadaptor.cpp \

HEADERS += \
    textprocessor.h \
    textprocessorifadaptor.h\

QT += dbus gui

target.path = $$DUI_INSTALL_BIN

INSTALLS += \
    target \
