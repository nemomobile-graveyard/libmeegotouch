DUIROOT = ../..
include($$DUIROOT/mkspecs/common.pri)

DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include
DUISFWINCLUDE = $$DUIROOT/servicefw/include
INCLUDEPATH += . \
    $$DUISRCINCLUDE \
    $$DUISRC
QMAKE_LIBDIR += $$DUILIB
win32|macx { 
    macx { 
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework \
            dui
    }
    win32:LIBS += -lduicore0
}
else:LIBS += -lduicore

TEMPLATE = app
TARGET = lifecycle
target.path = /usr/bin
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH
CONFIG += qt \
    qdbus
CONFIG -= app_bundle
QT += svg \
    dbus
SOURCES += main.cpp \
    mainpage.cpp \
    anotherpage.cpp \
    containerpage.cpp \
    lifecycleapplication.cpp
HEADERS += mainpage.h \
    anotherpage.h \
    containerpage.h \
    lifecycleapplication.h

# to update the .pm files automatically when running "make"
# include(../../updateqm.pri)
# LifeCycle Demo UI style definition
view_configuration.path = $$DUI_THEME_DIR/../lifecycle/themes
view_configuration.files = style/lifecycle.conf
style_images.path = $$DUI_THEME_DIR/../lifecycle/themes/images
style_images.files = images/*.png \
    images/*.jpg
style_svg.path = $$DUI_THEME_DIR/../lifecycle/themes/svg
style_svg.files = images/*.svg
contacts.path = $$DUI_THEME_DIR/../lifecycle/themes/images/contacts
contacts.files = images/contacts/*
desktop_entry.path = /usr/share/applications
desktop_entry.files = lifecycle.desktop
services.target = .dummy
services.commands = touch \
    $$services.target
services.path = /usr/share/dbus-1/services
services.files = com.nokia.lifecycle.service
DEFINES += CONTACTS_DIR=\"\\\"$$contacts.path/\\\"\"

# Install instructions
INSTALLS += target \
    style_images \
    style_svg \
    view_configuration \
    desktop_entry \
    services
