TEMPLATE = app
TARGET = trackergrid
CONFIG += dui # Requires libdui to be installed
LIBS += -lqttracker # Requires libqttracker + tracker

# Input
SOURCES += main.cpp trackergridpage.cpp
HEADERS += trackergridpage.h

# DUIROOT = ../..
#
# include($$DUIROOT/mkspecs/common.pri)
#
# DUILIB = $$DUIROOT/lib
# DUISRC = $$DUIROOT/src
# DUISRCINCLUDE = $$DUISRC/include
# DUISFWINCLUDE = $$DUIROOT/servicefw/include
#
# INCLUDEPATH += . \
#     $$DUISRCINCLUDE \
#     $$DUISRC \
#
# QMAKE_LIBDIR += \
#    $$DUILIB \
#
# INCLUDEPATH += ../../src/include
# QMAKE_LIBDIR += ../../lib/
#
# style_sheet.path = $$DUI_THEME_DIR/trackergrid
# style_sheet.files = style/trackergrid.css
# view_configuration.path = $$DUI_THEME_DIR/trackergrid
#
# INSTALLS += \
#    style_sheet \

