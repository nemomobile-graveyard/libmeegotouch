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

LIBS += $$mAddLibrary(meegotouchcore) \
        $$mAddLibrary(meegotouchviews) \
        $$mAddLibrary(meegotouchsettings) \
        $$mAddLibrary(meegotouchextensions)

QT += core \
    network \
    gui \
    svg \
    dbus \
    xml

# Input
SOURCES += main.cpp \
    mapplicationextensionrunner.cpp \
    ../src/extensions/applicationextension/mapplicationextensionloader.cpp

HEADERS += mapplicationextensionrunner.h \
    mapplicationextensionloader.h

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)
target.path = $$M_APPLICATION_EXTENSION_DIR
INSTALLS += target
