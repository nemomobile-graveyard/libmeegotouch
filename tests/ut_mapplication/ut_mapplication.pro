include(../common_top.pri)

TARGET = ut_mapplication

#holger FIXME check DEFINES += THEMEDIR=\'$$quote(\"$$[QT_INSTALL_DATA]/themes/m/\")\'

#holger FIXME check DEFINES += TRANSLATION_DIR=\'$$quote(\"$$[QT_INSTALL_DATA]/share/l10n/m/\")\'

TEST_HEADERS = $$MSRCDIR/corelib/core/mapplication.h

INCLUDEPATH += $$MSRCDIR/corelib/feedback

TEST_SOURCES = \
    $$MSRCDIR/corelib/core/mapplication.cpp

# Input
HEADERS += \
    ut_mapplication.h \
    $$TEST_HEADERS

SOURCES += \
    ut_mapplication.cpp \
    $$TEST_SOURCES \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

include(../common_bot.pri)
