MROOT = ../..
include($$MROOT/mkspecs/common.pri)
MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
MSFWINCLUDE = $$MROOT/servicefw/include
INCLUDEPATH += . \
    $$MSRCINCLUDE \
    $$MSRC/corelib

QMAKE_LIBDIR += $$MLIB
win32|macx { 
    macx { 
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework meegotouchcore \
                -framework meegotouchviews
    }
    win32:LIBS += -lmeegotouchcore0 \
                  -lmeegotouchviews0
}
else:LIBS += \
    -lmeegotouchcore \
    -lmeegotouchviews \
    -lmeegotouchsettings \
    -lmeegotouchextensions \

TEMPLATE = app
TARGET = widgetsgallery
target.path = $$M_INSTALL_BIN
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
    mainpage.cpp \
    labelpage.cpp \
    pushbuttonpage.cpp \
    buttongrouppage.cpp \
    feedbackpage.cpp \
    iconbuttonpage.cpp \
    switchpage.cpp \
    imagepage.cpp \
    applicationmenupage.cpp \
    toolbarpage.cpp \
    textentrypage.cpp \
    mlistpage.cpp \
    mgridpage.cpp \
    gridmodel.cpp \
    sliderpage.cpp \
    comboboxpage.cpp \
    progressbarpage.cpp \
    spinnerpage.cpp \
    checkboxpage.cpp \
    containerpage.cpp \
    objectmenupage.cpp \
    displaymodespage.cpp \
    languagepage.cpp \
    widgetsgalleryretranslator.cpp \
    utils.cpp \
    panningbenchmark.cpp \
    timedemo.cpp \
    ../../benchmarks/performancebenchmark/emptymainloophelper.cpp \
    timedemopage.cpp \
    timedemobenchmark.cpp \
    staticpagebenchmark.cpp \
    gridimagewidget.cpp \
    itemdetailpage.cpp \
    gridvideowidget.cpp \
    swaphook.cpp \
    maincategorypage.cpp \
    contentitemspage.cpp \
    bannerspage.cpp \
    querydialogspage.cpp \
    singleselectiondialogspage.cpp \
    phonebookcell.cpp \
    bubblepage.cpp \
    bubblelistmodel.cpp \

HEADERS += templatepage.h \
    mainpage.h \
    labelpage.h \
    pushbuttonpage.h \
    buttongrouppage.h \
    feedbackpage.h \
    iconbuttonpage.h \
    switchpage.h \
    imagepage.h \
    applicationmenupage.h \
    toolbarpage.h \
    textentrypage.h \
    mlistpage.h \
    mgridpage.h \
    gridmodel.h \
    sliderpage.h \
    comboboxpage.h \
    progressbarpage.h \
    spinnerpage.h \
    checkboxpage.h \
    containerpage.h \
    objectmenupage.h \
    displaymodespage.h \
    languagepage.h \
    widgetsgalleryretranslator.h \
    utils.h \
    panningbenchmark.h \
    timedemo.h \
    ../../benchmarks/performancebenchmark/emptymainloophelper.h \
    timedemopage.h \
    timedemobenchmark.h \
    staticpagebenchmark.h \
    gridimagewidget.h \
    itemdetailpage.h \
    gridvideowidget.h \
    swaphook.h \
    maincategorypage.h \
    contentitemspage.h \
    bannerspage.h \
    querydialogspage.h \
    singleselectiondialogspage.h \
    phonebookcell.h \
    bubblepage.h \
    bubblelistmodel.h \

# theme
include(theme/theme.pri)

contains( DEFINES, HAVE_N900 ) {
    SOURCES += contactmodel.cpp
    HEADERS += contactmodel.h

    PKGCONFIG += libosso-abook-1.0
    CONFIG += release link_pkgconfig

    desktop_icon.path = $$M_INSTALL_DATA/icons/hicolor/48x48/hildon
    desktop_icon.files = icon-l-widgetsgallery.png

    desktop_entry.path = $$M_INSTALL_DATA/applications/hildon
} else {
    SOURCES += phonebookmodel.cpp
    HEADERS += phonebookmodel.h

    desktop_icon.path = $$M_THEME_DIR/base/meegotouch/icons
    desktop_icon.files = icon-l-widgetsgallery.svg

    desktop_entry.path = $$M_INSTALL_DATA/applications
}
desktop_entry.files = widgetsgallery.desktop

myname = com.nokia.widgetsgallery
services.CONFIG += no_check_exist
services.target = $${myname}.service
services.commands = cat \
    $$IN_PWD/$${myname}.service.in \
    | \
    sed \
    -e \
    "s:@M_INSTALL_BIN@:$${M_INSTALL_BIN}:g" \
    > \
    $${myname}.service
services.files = $$OUT_PWD/$${myname}.service
services.path = $$M_DBUS_SERVICES_DIR
testsxml.files = tests.xml
testsxml.path = $$M_INSTALL_DATA/meegotouch-demos-widgetsgallery-tests/
DEFINES += CONTACTS_DIR=\"\\\"$$M_THEME_DIR/devel/meegotouch/widgetsgallery/images/contacts/\\\"\"
DEFINES += IMAGES_DIR=\"\\\"$$M_THEME_DIR/devel/meegotouch/widgetsgallery/images/\\\"\"
DEFINES += MEDIA_DIR=\"\\\"$$M_INSTALL_DATA/widgetsgallery/media\\\"\"

data_files.files = data/*
data_files.path = $$M_INSTALL_DATA/widgetsgallery/
data_files.CONFIG += no_check_exist

# Install instructions
INSTALLS += target \
    data_files \
    desktop_entry \
    testsxml \
    desktop_icon
!win32:!macx:INSTALLS += services
