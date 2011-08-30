include(../common_top.pri)

TARGET = ut_mapplication

#holger FIXME check DEFINES += THEMEDIR=\'$$quote(\"$$[QT_INSTALL_DATA]/themes/m/\")\'

#holger FIXME check DEFINES += TRANSLATION_DIR=\'$$quote(\"$$[QT_INSTALL_DATA]/share/l10n/m/\")\'

TEST_HEADERS = $$MSRCDIR/corelib/core/mapplication.h

INCLUDEPATH += \
        $$MSRCDIR/corelib/widgets \
        $$MSRCDIR/corelib/scene \
        $$MSRCDIR/corelib/core \
        $$MSRCDIR/corelib/style \
        $$MSRCDIR/corelib/events \
#        $$MSRCDIR/corelib/feedback

TEST_SOURCES = \
    $$MSRCDIR/corelib/core/mapplication.cpp

# Input
HEADERS += \
    ut_mapplication.h \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.h \
    $$TEST_HEADERS

SOURCES += \
    ut_mapplication.cpp \
    $$MSRCDIR/corelib/core/mdynamicpropertywatcher.cpp \
    $$TEST_SOURCES \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

include(../common_bot.pri)
