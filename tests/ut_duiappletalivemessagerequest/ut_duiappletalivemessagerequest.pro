include(../common_top.pri)
TARGET = ut_duiappletalivemessagerequest
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletalivemessagerequest.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletalivemessagerequest.h \

include(../common_bot.pri)
