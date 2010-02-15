include(../common_top.pri)
TARGET = ut_duiappletalivemessageresponse
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletalivemessageresponse.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletalivemessageresponse.h \

include(../common_bot.pri)
