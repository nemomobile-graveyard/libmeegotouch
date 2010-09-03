include( ../common.pri )

CONFIG += qt link_pkgconfig debug
TEMPLATE = app
TARGET = apscanner

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

