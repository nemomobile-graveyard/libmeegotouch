include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = ../dui-servicefwgen
DEPENDPATH += .
INCLUDEPATH += .

QT += xml

# Input
SOURCES += dui-servicefwgen.cpp

target.path = $$DUI_INSTALL_BIN
INSTALLS += target
