TEMPLATE = app
CONFIG += dui

# Input
SOURCES += calculator.cpp calculatorbutton.cpp  calculatorwidget.cpp
HEADERS += *.h

INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/

# The following lines are only to allow building the
# example inside the source tree without installing
# libdui first:
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
LIBS += -ldui
