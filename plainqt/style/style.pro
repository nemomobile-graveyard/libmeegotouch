MROOT = ../..

include($$MROOT/mkspecs/common.pri)

system(pkg-config --exists qttracker):HAVE_TRACKER=TRUE

#contains( HAVE_TRACKER, TRUE ) \
#: exists( $$[QMAKE_MKSPECS]/features/contentsearchinterface-maemo-meegotouch.prf) \
#: exists( $$[QMAKE_MKSPECS]/features/contentmanager.prf ) {
#    message("Using the Native File Dialog")
#    DEFINES += HAVE_NATIVE_FILEDIALOG
#}
#else {
#    message("Build-Dependencies for Native File Dialog are missing! " )
#    message("Please ensure that these packages are installed:" )
#    message("libqttracker-dev, libcontentmanager-dev, maemo-meegotouch-interfaces)")
#}

include(style.pri)

MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
MSFWINCLUDE = $$MROOT/servicefw/include

INCLUDEPATH += . \
    $$MSRCINCLUDE \
    $$MROOT/src/corelib/style \

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework m
    }
    win32:LIBS += -lmeegotouchcore0
} else {
    LIBS += -lmeegotouchcore -lmeegotouchviews
}
contains(DEFINES, HAVE_CONTEXTSUBSCRIBER) {
    LIBS += -lcontextsubscriber
    INCLUDEPATH += /usr/include/contextsubscriber
}

contains( DEFINES, HAVE_NATIVE_FILEDIALOG ) {
    LIBS += -lmaemomeegotouchcontentsearchinterface -lqttracker
}

TEMPLATE = lib
  CONFIG += plugin

TARGET = meegotouchqtstyleplugin
target.path = $$[QT_INSTALL_PLUGINS]/styles

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

install_headers.path = $$M_INSTALL_HEADERS
install_headers.files = \
  $$PUBLIC_HEADERS

INSTALLS += target \
  install_headers

RESOURCES += style.qrc
