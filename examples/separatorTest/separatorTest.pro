TEMPLATE = app
TARGET = SeparatorTest
CONFIG += m # Requires libm to be installed

# Input
SOURCES += main.cpp SeparatorTestPage.cpp

MROOT = ../..

include($$MROOT/mkspecs/common.pri)

MLIB = $$MROOT/lib
MSRC = $$MROOT/src
MSRCINCLUDE = $$MSRC/include
MSFWINCLUDE = $$MROOT/servicefw/include

INCLUDEPATH += . \
    $$MSRCINCLUDE \
    $$MSRC \

QMAKE_LIBDIR += \
    $$MLIB \

INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/

style_sheet.path = $$M_THEME_DIR/SeparatorTest
style_sheet.files = style/SeparatorTest.css
view_configuration.path = $$M_THEME_DIR/SeparatorTest

INSTALLS += \
    style_sheet \
