MROOT = ../..
include($$MROOT/mkspecs/common.pri)

TEMPLATE = app
TARGET = imtoolbar
CONFIG += meegotouch 

DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += imtoolbarpage.h
SOURCES += imtoolbarpage.cpp main.cpp

install.files =  ./toolbar1.xml \
                ./toolbar2.xml
install.path = /usr/share/m/imtoolbars/
INSTALLS += install

