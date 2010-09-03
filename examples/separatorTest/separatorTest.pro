include(../../mkspecs/common.pri)

TEMPLATE = app
TARGET = SeparatorTest
CONFIG += meegotouch

LIBS += -lmeegotouchcore

# Input
SOURCES += main.cpp SeparatorTestPage.cpp

INCLUDEPATH += . ../../src ../../src/include

style_sheet.files = style/SeparatorTest.css
style_sheet.path = $$M_THEME_DIR/base/meegotouch/SeparatorTest/style

INSTALLS += \
    style_sheet \
