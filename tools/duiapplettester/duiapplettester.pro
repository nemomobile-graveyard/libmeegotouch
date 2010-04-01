include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include \
    ../../src/extensions/mashup/mashup

DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = duiapplettester

QT += xml

LIBS += \
    -lduicore \
    -lduiviews \
    -lduisettings \
    -lduiextensions \

# Input
HEADERS += duiapplettester.h \
    duiapplettesterwindow.h \
    ../../src/extensions/mashup/mashup/duiappletid.h
SOURCES += main.cpp \
    duiapplettester.cpp \
    duiapplettesterwindow.cpp \
    ../../src/extensions/mashup/mashup/duiappletid.cpp
target.path = $$DUI_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
