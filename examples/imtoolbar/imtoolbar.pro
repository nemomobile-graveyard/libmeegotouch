DUIROOT = ../..
include($$DUIROOT/mkspecs/common.pri)

TEMPLATE = app
TARGET = imtoolbar
CONFIG += dui # Requires libdui to be installed

DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += imtoolbarpage.h
SOURCES += imtoolbarpage.cpp main.cpp

install.files =  ./toolbar1.xml \
                ./toolbar2.xml
install.path = /usr/share/dui/virtual-keyboard/layouts/
INSTALLS += install

