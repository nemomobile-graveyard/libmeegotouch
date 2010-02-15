include(../common_top.pri)
TARGET = ut_duiappletmessagefactory
INCLUDEPATH += $$DUISRCDIR/mashup/appletcommunication $$DUISRCDIR/mashup/appletinterface  $$DUISRCDIR/mashup/mashup

# unit test and unit classes
SOURCES += \
    ut_duiappletmessagefactory.cpp \

# unit test and unit classes
HEADERS += \
    ut_duiappletmessagefactory.h \
  
include(../common_bot.pri)
