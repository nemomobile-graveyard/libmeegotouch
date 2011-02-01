include( ../../common.pri )

INCLUDEPATH += ../../../../src/include
QMAKE_LIBDIR += ../../../../lib/
LIBS += $$mAddLibrary(meegotouchcore)

DEPENDPATH += .
INCLUDEPATH += .

TEMPLATE = lib
TARGET   = chainingexampleinterface

system($${M_BUILD_TREE}/tools/m-servicefwgen -p com.nokia.ChainingExampleInterface)

SOURCES += \
    chainingexampleinterfaceproxy.cpp \
    chainingexampleinterface.cpp

HEADERS += \
    chainingexampleinterface.h \
    chainingexampleinterfaceproxy.h

QMAKE_CLEAN += *.cpp *.h *~ *.so*

# target installation
target.path = /usr/lib

# install headers
headers.path  = /usr/include/chainingexample/service/
headers.files = $$HEADERS

#install xml file
xml.files = com.nokia.chainingexampleinterface.xml
xml.path  = /usr/share/dbus-1/interfaces/

# install prf file
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = chainingexample-service.prf

INSTALLS += \
    target \
    headers \
    prf \
    xml \
