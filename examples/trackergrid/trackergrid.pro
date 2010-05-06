TEMPLATE = app
TARGET = trackergrid
CONFIG += meegotouch 
LIBS += -lqttracker # Requires libqttracker + tracker

# Input
SOURCES += main.cpp trackergridpage.cpp
HEADERS += trackergridpage.h

# MROOT = ../..
#
# include($$MROOT/mkspecs/common.pri)
#
# MLIB = $$MROOT/lib
# MSRC = $$MROOT/src
# MSRCINCLUDE = $$MSRC/include
# MSFWINCLUDE = $$MROOT/servicefw/include
#
# INCLUDEPATH += . \
#     $$MSRCINCLUDE \
#     $$MSRC \
#
# QMAKE_LIBDIR += \
#    $$MLIB \
#
# INCLUDEPATH += ../../src/include
# QMAKE_LIBDIR += ../../lib/
#
# style_sheet.path = $$M_THEME_DIR/trackergrid
# style_sheet.files = style/trackergrid.css
# view_configuration.path = $$M_THEME_DIR/trackergrid
#
# INSTALLS += \
#    style_sheet \

