include(../common_top.pri)

TARGET = ut_duiimagewidget

SOURCES += \
    ut_duiimagewidget.cpp \
    $$STUBSDIR/stubbase.cpp \

HEADERS += \
    ut_duiimagewidget.h \

support_files.files += ut_duiimagewidget-test.png

include(../common_bot.pri)
