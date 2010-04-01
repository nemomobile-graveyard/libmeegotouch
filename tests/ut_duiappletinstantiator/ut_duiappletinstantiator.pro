include(../common_top.pri)

INCLUDEPATH += \
    $$DUISRCDIR/extensions/mashup/appletinstallation

# unit test and unit classes
SOURCES += \
    ut_duiappletinstantiator.cpp \
    $$DUISRCDIR/extensions/mashup/appletinstallation/duiappletinstantiator.cpp

# unit test and unit classes
HEADERS += \
   ut_duiappletinstantiator.h \
   $$DUISRCDIR/extensions/mashup/appletinstallation/duiappletinstantiator_p.h \
   $$DUISRCDIR/extensions/mashup/appletinstallation/duiappletinstantiator.h

include(../common_bot.pri)
