include(../common_top.pri)
TARGET = ut_mappletinstancedata
INCLUDEPATH += \
    $$MSRCDIR/extensions/mashup/appletcommunication \
    $$MSRCDIR/extensions/mashup/appletinterface \
    $$MSRCDIR/extensions/mashup/mashup \
    $$MSRCDIR/corelib/widgets

# unit test and unit classes
SOURCES += \
    ut_mappletinstancedata.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancedata.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
    ut_mappletinstancedata.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancedata.h \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.h

include(../common_bot.pri)
