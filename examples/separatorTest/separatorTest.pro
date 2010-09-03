include( ../common.pri )
include( ../../mkspecs/common.pri )

TEMPLATE = app
TARGET = SeparatorTest

# Input
SOURCES += main.cpp SeparatorTestPage.cpp

style_sheet.files = style/SeparatorTest.css
style_sheet.path = $$M_THEME_DIR/base/meegotouch/SeparatorTest/style

INSTALLS += \
    style_sheet \
