include( ../common.pri )

TEMPLATE = app
TARGET = chainingexampleapp1

INCLUDEPATH += ../chainingexampleinterface
DEPENDPATH += $$INCLUDEPATH

LIBS += ../chainingexampleinterface/libchainingexampleinterface.so

# Input
SOURCES += \
    main.cpp \
    imageviewer.cpp \

HEADERS += \
    imageviewer.h \

# target installation
target.path = /usr/bin

#install desktop file
desktop.files = chainingexampleapp1.desktop
desktop.path  = /usr/share/applications/

# install prf file
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = chainingexampleapp1-service.prf

#install service file
service.files = com.nokia.chainingexampleapp1.service
service.path  = /usr/share/dbus-1/services/

INSTALLS += \
    target \
    prf \
    desktop \
    service \
