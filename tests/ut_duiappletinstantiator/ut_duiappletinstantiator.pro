include(../common_top.pri)
TARGET = ut_duiappletinstantiator
INCLUDEPATH += \
    $$DUISRCDIR/mashup/appletinstallation

# unit test and unit classes
SOURCES += \
    ut_duiappletinstantiator.cpp \
    $$DUISRCDIR/mashup/appletinstallation/duiappletinstantiator.cpp

# unit test and unit classes
HEADERS += \
   ut_duiappletinstantiator.h \
   $$DUISRCDIR/mashup/appletinstallation/duiappletinstantiator_p.h \
   $$DUISRCDIR/mashup/appletinstallation/duiappletinstantiator.h

include(../common_bot.pri)
