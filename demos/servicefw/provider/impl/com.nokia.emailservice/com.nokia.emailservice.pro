include( ../../../../../mkspecs/common.pri )

DESTDIR=../../../bin/
TARGET=com.nokia.emailservice
TEMPLATE=app
SOURCES += \
    main.cpp \
    emailservice.cpp \
    emailserviceifadaptor.cpp \

HEADERS += \
    emailservice.h \
    emailserviceifadaptor.h\

QT += dbus gui

target.path = $$DUI_INSTALL_BIN

INSTALLS += \
    target \
