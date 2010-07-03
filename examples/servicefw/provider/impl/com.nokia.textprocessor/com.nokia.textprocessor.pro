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

CONFIG += meegotouch

QT += gui

target.path = $$M_INSTALL_BIN

INSTALLS += \
    target \
