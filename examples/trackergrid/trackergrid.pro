include( ../common.pri )

TEMPLATE = app
TARGET = trackergrid
LIBS += -lqttracker # Requires libqttracker + tracker

# Input
SOURCES += main.cpp trackergridpage.cpp
HEADERS += trackergridpage.h

