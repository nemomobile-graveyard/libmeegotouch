TEMPLATE = app
TARGET = SeparatorTest
CONFIG += dui # Requires libdui to be installed

# Input
SOURCES += main.cpp SeparatorTestPage.cpp

DUIROOT = ../..

include($$DUIROOT/mkspecs/common.pri)

DUILIB = $$DUIROOT/lib
DUISRC = $$DUIROOT/src
DUISRCINCLUDE = $$DUISRC/include
DUISFWINCLUDE = $$DUIROOT/servicefw/include

INCLUDEPATH += . \
    $$DUISRCINCLUDE \
    $$DUISRC \

QMAKE_LIBDIR += \
    $$DUILIB \

INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/

style_sheet.path = $$DUI_THEME_DIR/SeparatorTest
style_sheet.files = style/SeparatorTest.css
view_configuration.path = $$DUI_THEME_DIR/SeparatorTest

INSTALLS += \
    style_sheet \
