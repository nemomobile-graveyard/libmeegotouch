QMAKE_CXXFLAGS += -Werror

MROOT = ../..

include($$MROOT/mkspecs/common.pri)

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
