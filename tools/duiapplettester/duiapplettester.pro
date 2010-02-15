include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include \
    ../../src/mashup/mashup
DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = duiapplettester

QT += xml

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -L../../lib -ldui0
} else {
    LIBS += ../../lib/libdui.so
}

# Input
HEADERS += duiapplettester.h \
    duiapplettesterwindow.h \
    ../../src/mashup/mashup/duiappletid.h
SOURCES += main.cpp \
    duiapplettester.cpp \
    duiapplettesterwindow.cpp \
    ../../src/mashup/mashup/duiappletid.cpp
target.path = $$DUI_INSTALL_BIN
INSTALLS += target
CONFIG -= app_bundle
