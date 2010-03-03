include(../mkspecs/common.pri)

INCLUDEPATH += . \
    ../src/mashup/mashup \
    ../src/include \
    ../src/mashup/appletcommunication \
    ../src/mashup/appletinterface \
    ../src/applicationextension
DEPENDPATH += $$INCLUDEPATH
QMAKE_LIBDIR += ../lib
TEMPLATE = app
TARGET =
DEPENDPATH += .
QMAKE_LFLAGS += ../lib/libdui.so
QT += core \
    network \
    gui \
    svg \
    dbus \
    xml

# Input
SOURCES += main.cpp \
    duiapplicationextensionrunner.cpp \
    ../src/applicationextension/duiapplicationextensionloader.cpp

HEADERS += duiapplicationextensionrunner.h \
    duiapplicationextensionloader.h

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)
target.path = $$DUI_APPLICATION_EXTENSION_DIR
INSTALLS += target
