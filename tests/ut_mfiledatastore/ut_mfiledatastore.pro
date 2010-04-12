include(../common_top.pri)
TARGET = ut_mfiledatastore
INCLUDEPATH += $$MSRCDIR/mashup/appletcommunication $$MSRCDIR/mashup/appletinterface  $$MSRCDIR/mashup/mashup

SOURCES += \
    ut_mfiledatastore.cpp \

HEADERS += \
    ut_mfiledatastore.h \

include(../common_bot.pri)
