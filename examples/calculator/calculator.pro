TEMPLATE = app
CONFIG += m

# Input
SOURCES += calculator.cpp calculatorbutton.cpp  calculatorwidget.cpp
HEADERS += *.h

INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/

# The following lines are only to allow building the
# example inside the source tree without installing
# libm first:
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
LIBS += -lmeegotouch
