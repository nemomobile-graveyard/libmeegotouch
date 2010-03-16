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
    win32:LIBS += -L../../lib \
        -ldui0
}
else:LIBS += ../../lib/libdui.so

TEMPLATE = app
TARGET = widgetsgallery
target.path = $$DUI_INSTALL_BIN
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
DEPENDPATH += $$INCLUDEPATH
CONFIG += qt
QT += svg
contains(DEFINES, HAVE_DBUS) {
    CONFIG += qdbus
    QT += dbus
}
SOURCES += main.cpp \
    templatepage.cpp \
    listpage.cpp \
    labelpage.cpp \
    pushbuttonpage.cpp \
    feedbackpage.cpp \
    iconbuttonpage.cpp \
    switchpage.cpp \
    dialogsandnotificationspage.cpp \
    imagepage.cpp \
    applicationmenupage.cpp \
    toolbarpage.cpp \
    textentrypage.cpp \
    duilistpage.cpp \
    duigridpage.cpp \
    gridmodel.cpp \
    sliderpage.cpp \
    comboboxpage.cpp \
    progressbarpage.cpp \
    spinnerpage.cpp \
    checkboxpage.cpp \
    containerpage.cpp \
    objectmenupage.cpp \
    navigationbarpage.cpp \
    languagepage.cpp \
    widgetsgalleryretranslator.cpp \
    utils.cpp \
    panningbenchmark.cpp \
    timedemo.cpp \
    timingscene.cpp \
    ../../benchmarks/performancebenchmark/emptymainloophelper.cpp \
    timedemopage.cpp \
    timedemobenchmark.cpp \
    staticpagebenchmark.cpp

HEADERS += templatepage.h \
    listpage.h \
    labelpage.h \
    pushbuttonpage.h \
    feedbackpage.h \
    iconbuttonpage.h \
    switchpage.h \
    dialogsandnotificationspage.h \
    imagepage.h \
    applicationmenupage.h \
    toolbarpage.h \
    textentrypage.h \
    duilistpage.h \
    duigridpage.h \
    gridmodel.h \
    sliderpage.h \
    comboboxpage.h \
    progressbarpage.h \
    spinnerpage.h \
    checkboxpage.h \
    containerpage.h \
    objectmenupage.h \
    navigationbarpage.h \
    languagepage.h \
    widgetsgalleryretranslator.h \
    utils.h \
    panningbenchmark.h \
    timedemo.h \
    timingscene.h \
    ../../benchmarks/performancebenchmark/emptymainloophelper.h \
    timedemopage.h \
    timedemobenchmark.h \
    staticpagebenchmark.h

contains(DEFINES, HAVE_GSTREAMER) {
  SOURCES += videocontainerpage.cpp
  HEADERS += videocontainerpage.h
}

# theme
include(theme/theme.pri)

contains( DEFINES, HAVE_N900 ) {
    SOURCES += contactmodel.cpp
    HEADERS += contactmodel.h

    PKGCONFIG += libosso-abook-1.0
    CONFIG += release link_pkgconfig

    desktop_icon.path = $$DUI_INSTALL_DATA/icons/hicolor/64x64/hildon
    desktop_icon.files = widgetsgallery.png

    INSTALLS += desktop_icon

    desktop_entry.path = $$DUI_INSTALL_DATA/applications/hildon
    desktop_entry.files = widgetsgallery-n900.desktop
} else {
    SOURCES += phonebookmodel.cpp
    HEADERS += phonebookmodel.h

    desktop_entry.path = $$DUI_INSTALL_DATA/applications
    desktop_entry.files = widgetsgallery.desktop
}

myname = com.nokia.widgetsgallery
services.CONFIG += no_check_exist
services.target = $${myname}.service
services.commands = cat \
    $$IN_PWD/$${myname}.service.in \
    | \
    sed \
    -e \
    "s:@DUI_INSTALL_BIN@:$${DUI_INSTALL_BIN}:g" \
    > \
    $${myname}.service
services.files = $$OUT_PWD/$${myname}.service
services.path = $$DUI_DBUS_SERVICES_DIR
testsxml.files = tests.xml
testsxml.path = /usr/share/dui-demos-widgetsgallery-tests/
DEFINES += CONTACTS_DIR=\"\\\"$$DUI_THEME_DIR/devel/dui/widgetsgallery/images/contacts/\\\"\"
DEFINES += IMAGES_DIR=\"\\\"$$DUI_THEME_DIR/devel/dui/widgetsgallery/images/\\\"\"

data_files.files = data/*
data_files.path = /usr/share/widgetsgallery/
data_files.CONFIG += no_check_exist

# Install instructions
INSTALLS += target \
    data_files \
    desktop_entry \
    testsxml
!win32:!macx:INSTALLS += services
