include(../common_top.pri)

INCLUDEPATH += $$MSRCDIR/extensions \
               $$MSRCDIR/extensions/mashup/mashup

TARGET = ut_mextensionwatcher

TEST_SOURCES = \
    $$MSRCDIR/extensions/mextensionwatcher.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletinstancedata.cpp \
    $$MSRCDIR/extensions/mashup/mashup/mappletid.cpp \
    $$MSRCDIR/extensions/applicationextension/mapplicationextensionmetadata.cpp


# unit test and unit
SOURCES += \
    ut_mextensionwatcher.cpp \
    $$TEST_SOURCES

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_mextensionwatcher.h \
    $$MSRCDIR/extensions/mextensionwatcher.h


include(../common_bot.pri)
