include(../common_top.pri)

INCLUDEPATH += \
    $$MSRCDIR/extensions/mashup/appletinstallation

# unit test and unit classes
SOURCES += \
    ut_mappletinstantiator.cpp \
    $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.cpp

# unit test and unit classes
HEADERS += \
   ut_mappletinstantiator.h \
   $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator_p.h \
   $$MSRCDIR/extensions/mashup/appletinstallation/mappletinstantiator.h

include(../common_bot.pri)
