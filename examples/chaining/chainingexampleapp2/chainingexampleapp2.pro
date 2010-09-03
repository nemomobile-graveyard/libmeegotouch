include( ../common.pri )

TEMPLATE = app
TARGET = chainingexampleapp2

system(cp ../chainingexampleinterface/com.nokia.ChainingExampleInterface.xml .)
system($${M_BUILD_TREE}/tools/m-servicefwgen -a com.nokia.ChainingExampleInterface)

QMAKE_CLEAN += \
    com.nokia.ChainingExampleInterface.xml \
    chainingexampleinterfaceadaptor.cpp \
    chainingexampleinterfaceadaptor.h \

# Input
SOURCES += \
    main.cpp \
    imageviewer.cpp \
    chainingexampleinterfaceadaptor.cpp \

HEADERS += \
    imageviewer.h \
    chainingexampleinterfaceadaptor.h \

# target installation
target.path = /usr/bin

#install service file
service.files = com.nokia.ChainingExampleApp2.service
service.path  = /usr/share/dbus-1/services/

#install desktop file
desktop.files = chainingexampleapp2.desktop
desktop.path  = /usr/share/applications/

# install prf file
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = chainingexampleapp2-service.prf

INSTALLS += \
    target \
    service \
    prf \
    desktop \
