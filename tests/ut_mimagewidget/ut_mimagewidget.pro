include(../common_top.pri)

TARGET = ut_mimagewidget

SOURCES += \
    ut_mimagewidget.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_mimagewidget.h \

support_files.files += ut_mimagewidget-test.png

include(../common_bot.pri)
