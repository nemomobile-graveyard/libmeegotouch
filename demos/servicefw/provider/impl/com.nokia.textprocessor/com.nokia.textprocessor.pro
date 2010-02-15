include( ../../../../../mkspecs/common.pri )

DESTDIR=../../../bin/
TARGET=com.nokia.textprocessor
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
