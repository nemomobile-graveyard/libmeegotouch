TEMPLATE = app
TARGET = helloworld
CONFIG += m # Requires libm to be installed

# Input
SOURCES += main.cpp

# The following lines are only to allow building the
# example inside the source tree without installing
# libm first:
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
LIBS += -lmeegotouch
