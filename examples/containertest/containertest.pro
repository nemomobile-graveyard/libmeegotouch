TEMPLATE = app
TARGET = containertest
CONFIG += dui # Requires libdui to be installed

# Input
SOURCES += main.cpp

INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/

# The following lines are only to allow building the
# example inside the source tree without installing
# libdui first:
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
LIBS += -ldui
