MROOT=../../../../..
include($$MROOT/mkspecs/common.pri)

DESTDIR=$$MROOT/bin/
TARGET=org.maemo.textprocessor
TEMPLATE=app
SOURCES += \
    main.cpp \
    textprocessor.cpp \
    textprocessorinterfaceadaptor.cpp \

HEADERS += \
    textprocessor.h \
    textprocessorinterfaceadaptor.h\

QT += gui dbus

target.path = $$M_INSTALL_BIN

INSTALLS += \
    target \
