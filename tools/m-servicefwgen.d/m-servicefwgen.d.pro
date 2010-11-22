include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = m-servicefwgen
DESTDIR = $$M_BUILD_TREE/tools

QT += xml

# Input
SOURCES += m-servicefwgen.cpp

target.path = $$M_INSTALL_BIN
INSTALLS += target
