include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include \
    ../../src/extensions/mashup/mashup

DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = mapplettester

QT += xml

LIBS += \
    -lmeegotouchcore \
    -lmeegotouchviews \
    -lmeegotouchsettings \
    -lmeegotouchextensions \

# Input
HEADERS += mapplettester.h \
    mapplettesterwindow.h \
    ../../src/extensions/mashup/mashup/mappletid.h
SOURCES += main.cpp \
    mapplettester.cpp \
    mapplettesterwindow.cpp \
    ../../src/extensions/mashup/mashup/mappletid.cpp
target.path = $$M_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
