include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include
DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = mapplicationextensiontester

LIBS += \
    -lmeegotouchcore \
    -lmeegotouchviews \
    -lmeegotouchsettings \
    -lmeegotouchextensions \

# Input
HEADERS += mapplicationextensiontester.h

SOURCES += mapplicationextensiontester.cpp

target.path = $$M_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
