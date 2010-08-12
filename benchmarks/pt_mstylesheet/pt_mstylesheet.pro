include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/include
INCLUDEPATH += $$MSRCDIR/corelib/theme
INCLUDEPATH += $$MSRCDIR/corelib/style
DEPENDPATH += $$INCLUDEPATH
TARGET = pt_mstylesheet

SOURCES += pt_mstylesheet.cpp \
           mlogicalvalues.cpp \
           mstylesheetattribute.cpp \

HEADERS += pt_mstylesheet.h \
           mlogicalvalues.h \
           mstylesheetattribute.h \
