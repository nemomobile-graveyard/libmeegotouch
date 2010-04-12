include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = ../m-servicefwgen
DEPENDPATH += .
INCLUDEPATH += .

QT += xml

# Input
SOURCES += m-servicefwgen.cpp

target.path = $$M_INSTALL_BIN
INSTALLS += target
