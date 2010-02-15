CONFIG += dui qt link_pkgconfig debug
TEMPLATE = app
TARGET = apscanner
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += main.cpp \
           apscanner.cpp \
           apscannerpage.cpp \
           apitemmodel.cpp \
           apitemcreator.cpp
           
HEADERS += apscanner.h \
           apscannerpage.h \
           apitemmodel.h \
           apitemcreator.h

# The following lines are only to allow building the
# example inside the source tree without installing
# libdui first:
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
LIBS += -ldui
