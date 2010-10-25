include(../mkspecs/common.pri)

INCLUDEPATH += . \
    ../src/extensions/mashup/mashup \
    ../src/include \
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
    mappletrunner.cpp

HEADERS += mappletrunner.h

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)
target.path = $$M_APPLET_DIR
INSTALLS += target
