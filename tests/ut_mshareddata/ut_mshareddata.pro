include(../common_top.pri)
INCLUDEPATH += $$MSRCDIR/corelib/theme

TARGET = ut_mshareddata

# unit test and unit
SOURCES += \
    ut_mshareddata.cpp \
    
# unit test and unit
HEADERS += \
    ut_mshareddata.h \

include(../common_bot.pri)

