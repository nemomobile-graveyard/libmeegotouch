QMAKE_CXXFLAGS += -Werror

DUIROOT = ../..

include($$DUIROOT/mkspecs/common.pri)

include(style.pri)

DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include
DUISFWINCLUDE = $$DUIROOT/servicefw/include

INCLUDEPATH += . \
    $$DUISRCINCLUDE \
    $$DUISRC \

QMAKE_LIBDIR += \
    $$DUILIB \

win32|macx {
    macx {
        QMAKE_LFLAGS += -F../../lib
        LIBS += -framework dui
    }
    win32:LIBS += -L../../lib -ldui0
} else {
    LIBS += ../../lib/libdui.so
}

TEMPLATE = lib
  CONFIG += plugin

TARGET = duiqtstyleplugin
target.path = $$[QT_INSTALL_PLUGINS]/styles

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

install_headers.path = $$DUI_INSTALL_HEADERS
install_headers.files = \
  $$PUBLIC_HEADERS

INSTALLS += target \
  install_headers

RESOURCES += style.qrc
