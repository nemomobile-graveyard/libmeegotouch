include(../common_top.pri)

TARGET = ut_duiapplication

#holger FIXME check DEFINES += THEMEDIR=\'$$quote(\"$$[QT_INSTALL_DATA]/themes/dui/\")\'

#holger FIXME check DEFINES += TRANSLATION_DIR=\'$$quote(\"$$[QT_INSTALL_DATA]/share/l10n/dui/\")\'

TEST_HEADERS = $$DUISRCDIR/corelib/core/duiapplication.h

INCLUDEPATH += $$DUISRCDIR/corelib/feedback

TEST_SOURCES = \
    $$DUISRCDIR/corelib/core/duiapplication.cpp

# Input
HEADERS += \
    ut_duiapplication.h \
    $$TEST_HEADERS

SOURCES += \
    ut_duiapplication.cpp \
    $$TEST_SOURCES \

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

include(../common_bot.pri)
