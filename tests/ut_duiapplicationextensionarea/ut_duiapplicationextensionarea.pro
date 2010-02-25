include(../common_top.pri)
TARGET = ut_duiapplicationextensionarea
INCLUDEPATH += \
    $$DUISRCDIR/applicationextension \
    $$DUISRCDIR/mashup/mashup \
    $$DUISRCDIR/widgets \
    $$DUISRCDIR/style \
    $$DUISRCDIR/core \

# unit test and unit classes
SOURCES += \
    ut_duiapplicationextensionarea.cpp \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit classes
HEADERS += \
   ut_duiapplicationextensionarea.h \

include(../common_bot.pri)
