include(../mkspecs/common.pri)

INCLUDEPATH += . \
    ../src/include \
    ../src/extensions/mashup/mashup \
    ../src/extensions/mashup/appletcommunication \
    ../src/extensions/mashup/appletinterface \
    ../src/extensions/applicationextension

DEPENDPATH += $$INCLUDEPATH
QMAKE_LIBDIR += ../lib
TEMPLATE = app
TARGET =
DEPENDPATH += .

LIBS += \
    -lduicore \
    -lduiviews \
    -lduisettings \
    -lduiextensions \

QT += core \
    network \
    gui \
    svg \
    dbus \
    xml

# Input
SOURCES += main.cpp \
    duiapplicationextensionrunner.cpp \
    ../src/extensions/applicationextension/duiapplicationextensionloader.cpp

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
