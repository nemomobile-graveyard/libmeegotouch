include(../../mkspecs/common.pri)

INCLUDEPATH += . \
    ../../src/include \
    ../../src/extensions/mashup/mashup

DEPENDPATH += $$INCLUDEPATH
TEMPLATE = app
TARGET = mapplettester

QT += xml

LIBS += $$mAddLibrary(meegotouchcore) \
        $$mAddLibrary(meegotouchviews) \
        $$mAddLibrary(meegotouchsettings) \
        $$mAddLibrary(meegotouchextensions)

# Input
HEADERS += mapplettester.h \
    mapplettesterwindow.h \
    ../../src/extensions/mashup/mashup/mappletid.h
SOURCES += main.cpp \
    mapplettester.cpp \
    mapplettesterwindow.cpp \
    ../../src/extensions/mashup/mashup/mappletid.cpp
target.path = $$M_INSTALL_BIN
INSTALLS += target style
CONFIG -= app_bundle

style.path = $$M_THEME_DIR/base/meegotouch/mapplettester/style
style.files = *.css
