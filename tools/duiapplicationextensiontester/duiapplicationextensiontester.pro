include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include
DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = duiapplicationextensiontester

LIBS += \
    -lduicore \
    -lduiviews \
    -lduisettings \
    -lduiextensions \

# Input
HEADERS += duiapplicationextensiontester.h

SOURCES += duiapplicationextensiontester.cpp

target.path = $$DUI_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
