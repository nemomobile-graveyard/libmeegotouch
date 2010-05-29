TEMPLATE = app
TARGET = focus
CONFIG += meegotouch

# Input
SOURCES += main.cpp \
    mydialog.cpp \
    mypage.cpp

HEADERS += \
    mydialog.h \
    mypage.h

# The following lines are only to allow building the
# example inside the source tree without installing
# libmeegotouch first:
INCLUDEPATH += ../../src/include
QMAKE_LIBDIR += ../../lib/
LIBS += -lmeegotouchcore
