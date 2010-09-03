include( ../common.pri )

TEMPLATE = app
TARGET = imtoolbar

# Input
HEADERS += imtoolbarpage.h
SOURCES += imtoolbarpage.cpp main.cpp

install.files = toolbar1.xml \
                toolbar2.xml
install.path = /usr/share/meegotouch/imtoolbars/
INSTALLS += install
