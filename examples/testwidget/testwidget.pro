TEMPLATE = app
TARGET = testwidget
CONFIG += meegotouch 

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
