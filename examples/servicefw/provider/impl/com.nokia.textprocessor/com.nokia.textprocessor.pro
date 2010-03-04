include( ../../../../../mkspecs/common.pri )

DESTDIR=../../../bin/
TARGET=com.nokia.textprocessor
TEMPLATE=app
SOURCES += \
    main.cpp \
    textprocessor.cpp \
    textprocessorinterfaceadaptor.cpp \

HEADERS += \
    textprocessor.h \
    textprocessorinterfaceadaptor.h\

QT += dbus gui

target.path = $$DUI_INSTALL_BIN

INSTALLS += \
    target \
