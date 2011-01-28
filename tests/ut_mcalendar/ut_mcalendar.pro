include(../common_top.pri)

TARGET = ut_mcalendar

ICUDATA = $$system(icu-config --icudata)      # for example “icudt44l”
ICUUSRDATA = $$replace(ICUDATA, icudt, usrdt) # for example “usrdt44l”
DEFINES += ICUUSRDATA=\"\\\"$$ICUUSRDATA\\\"\"

TEST_SOURCES +=  \
#    $$MSRCDIR/mcalendar.cpp \
 
# Input
HEADERS += ut_mcalendar.h
SOURCES += ut_mcalendar.cpp

include(../common_bot.pri)
