TEMPLATE = app
TARGET = testwidget
CONFIG += dui # Requires libdui to be installed

# Input
SOURCES += testwidgetmodel.cpp \
    main.cpp \
    testwidget.cpp \
    testwidgetview.cpp
HEADERS += testwidget.h \
    testwidgetmodel.h \
    testwidgetstyle.h \
    testwidgetview.h
MODEL_HEADERS += testwidgetmodel.h
STYLE_HEADERS += testwidgetstyle.h
